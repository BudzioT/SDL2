#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

#include "LTexture.h"

#include <stdio.h>
#include <string>
#include <sstream>


/* Initialize the program */
bool init();
/* Load all the needed media */
bool loadMedia();
/* Clean up */
void close();

/* Recording/playback callbacks */
void audioRecordingCallback(void* userData, Uint8* stream, int len);
void audioPlaybackCallback(void* userData, Uint8* stream, int len);


/* Screen dimensions */
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

/* Global window and renderer */
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

/* Global font */
TTF_Font* font = nullptr;
SDL_Color textColor = { 0, 0, 0, 255 };


/* Maximum number of supported recording devices */
const int MAX_RECORDING_DEVICES = 10;
/* Maximum recording time */
const int MAX_RECORDING_TIME = 5;
/* Maximum recording time plus padding */
const int RECORDING_BUFFER_SECONDS = MAX_RECORDING_TIME + 1;


/* Text textures */
LTexture promptText;
LTexture deviceText[MAX_RECORDING_DEVICES];


/* The various recording actions */
enum class RecordingState {
	SELECTING_DEVICE,
	STOPPED,
	RECORDING,
	RECORDED,
	PLAYBACK,
	ERROR
};

/* Number of available devices */
int recordingDeviceCount = 0;

/* Recived audio spec */
SDL_AudioSpec recivedRecordingSpec;
SDL_AudioSpec recivedPlaybackSpec;


/* Recording data buffer */
Uint8* recordingBuffer = nullptr;
/* Size of data buffer */
Uint32 bufferByteSize = 0;
/* Position in data buffer */
Uint32 bufferBytePosition = 0;
/* Maximum position in data buffer for recording */
Uint32 bufferByteMaxPosition = 0;


int main(int argc, char* args[])
{
	/* Initialize */
	if (!init()) {
		printf("Failed to initialize!\n");
		close();
		return -1;
	}

	/* Load media */
	if (!loadMedia()) {
		printf("Failed to load media!\n");
		close();
		return -1;
	}


	bool quit = false;
	SDL_Event e;

	/* Set the default recording state */
	RecordingState currentState = RecordingState::SELECTING_DEVICE;

	/* Audio devices IDs */
	SDL_AudioDeviceID recordingDeviceID = 0;
	SDL_AudioDeviceID playbackDeviceID = 0;


	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				quit = true;

			/* Do current state event handling */
			switch (currentState) {
				/* User is selecting recording device */
			case RecordingState::SELECTING_DEVICE:
				/* On key press*/
				if (e.type == SDL_KEYDOWN) {
					/* Handle key press from 0 to 9 */
					if (e.key.keysym.sym >= SDLK_0 && e.key.keysym.sym <= SDLK_9) {
						/* Get selection index */
						int index = e.key.keysym.sym - SDLK_0;
						
						/* Index is valid */
						if (index < recordingDeviceCount) {
							/* Default audio spec */
							SDL_AudioSpec desiredRecordingSpec;
							SDL_zero(desiredRecordingSpec);
							desiredRecordingSpec.freq = 44100;
							desiredRecordingSpec.format = AUDIO_F32;
							desiredRecordingSpec.channels = 2;
							desiredRecordingSpec.samples = 4096;
							desiredRecordingSpec.callback = audioRecordingCallback;

							/* Open recording device */
							recordingDeviceID = SDL_OpenAudioDevice(SDL_GetAudioDeviceName(index, SDL_TRUE),
								SDL_TRUE, &desiredRecordingSpec, &recivedRecordingSpec,
								SDL_AUDIO_ALLOW_FORMAT_CHANGE);
							/* Device failed to open */
							if (recordingDeviceID == 0) {
								/* Repeat error */
								printf("Couldn't open recording device! SDL_Error: %s\n", SDL_GetError());
								promptText.loadFromRenderedText("Failed to open recording device!", textColor);
								currentState = RecordingState::ERROR;
							}
							/* Device opened successfully */
							else {
								/* Default audio spec */
								SDL_AudioSpec desiredPlaybackSpec;
								SDL_zero(desiredPlaybackSpec);
								desiredPlaybackSpec.freq = 44100;
								desiredPlaybackSpec.format = AUDIO_F32;
								desiredPlaybackSpec.channels = 2;
								desiredPlaybackSpec.samples = 4096;
								desiredPlaybackSpec.callback = audioPlaybackCallback;

								/* Open playback device */
								playbackDeviceID = SDL_OpenAudioDevice(NULL, SDL_FALSE, &desiredPlaybackSpec, 
									&recivedPlaybackSpec, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
								/* Device failed to open */
								if (playbackDeviceID == 0) {
									/* Repeat error */
									printf("Couldn't open playback device! SDL_Error: %s\n", SDL_GetError());
									promptText.loadFromRenderedText("Failed to open playback device!", 
										textColor);
									currentState = RecordingState::ERROR;
								}
								/* Device opened successfully */
								else {
									/* Calculate per sample bytes */
									int bytesPerSample = recivedRecordingSpec.channels * 
										(SDL_AUDIO_BITSIZE(recivedRecordingSpec.format) / 8);

									/* Calculate bytes per second */
									int bytesPerSecond = recivedRecordingSpec.freq * bytesPerSample;

									/* Calculate buffer size */
									bufferByteSize = RECORDING_BUFFER_SECONDS * bytesPerSecond;

									/* Calculate max buffer use */
									bufferByteMaxPosition = MAX_RECORDING_TIME * bytesPerSecond;

									/* Allocate and initialize byte buffer */
									recordingBuffer = new Uint8[bufferByteSize];
									memset(recordingBuffer, 0, bufferByteSize);

									/* Go on to next state */
									promptText.loadFromRenderedText("Press 1 to record for 5 seconds.", 
										textColor);
									currentState = RecordingState::STOPPED;
								}
							}
						}
					}
				}
				break;

				/* User getting ready to record */
			case RecordingState::STOPPED:
				/* On key press */
				if (e.type == SDL_KEYDOWN) {
					/* Start recording */
					if (e.key.keysym.sym == SDLK_1) {
						/* Go back to begging of the buffer */
						bufferBytePosition = 0;

						/* Start recording */
						SDL_PauseAudioDevice(recordingDeviceID, SDL_FALSE);

						/* Go on to next state */
						promptText.loadFromRenderedText("Recording...", textColor);
						currentState = RecordingState::RECORDING;
					}
				}
				break;
				
				/* User finished recording */
			case RecordingState::RECORDED:
				/* On key press */
				if (e.type == SDL_KEYDOWN) {
					/* Start playback */
					if (e.key.keysym.sym == SDLK_1) {
						/* Go back to the beggining of the buffer */
						bufferBytePosition = 0;

						/* Start playback */
						SDL_PauseAudioDevice(playbackDeviceID, SDL_FALSE);

						/* Go on to next state */
						promptText.loadFromRenderedText("Playing...", textColor);
						currentState = RecordingState::PLAYBACK;
					}

					/* Record again */
					if (e.key.keysym.sym == SDLK_2) {
						/* Reset the buffer */
						bufferBytePosition = 0;

						/* Start recording */
						SDL_PauseAudioDevice(recordingDeviceID, SDL_FALSE);

						/* Go on to next state */
						promptText.loadFromRenderedText("Recording...", textColor);
						currentState = RecordingState::RECORDING;
					}
				}
				break;
			}
		}

		/* Update recording */
		if (currentState == RecordingState::RECORDING) {
			/* Lock callback */
			SDL_LockAudioDevice(recordingDeviceID);

			/* Finished recording */
			if (bufferBytePosition > bufferByteMaxPosition) {
				/* Stop recording audio */
				SDL_PauseAudioDevice(recordingDeviceID, SDL_TRUE);

				/* Go on to next state */
				promptText.loadFromRenderedText("Press 1 to play back. Press 2 to record again.", textColor);
				currentState = RecordingState::RECORDED;
			}

			/* Unlock callback */
			SDL_UnlockAudioDevice(recordingDeviceID);
		}

		/* Update playback */
		else if (currentState == RecordingState::PLAYBACK) {
			/* Lock callback */
			SDL_LockAudioDevice(playbackDeviceID);

			/* Finished playback */
			if (bufferBytePosition > bufferByteMaxPosition) {
				/* Stop playing audio */
				SDL_PauseAudioDevice(playbackDeviceID, SDL_TRUE);

				/* Gon to next state */
				promptText.loadFromRenderedText("Press 1 to play back. Press 2 to record again.", textColor);
				currentState = RecordingState::RECORDED;
			}

			/* Unlock callback */
			SDL_UnlockAudioDevice(playbackDeviceID);
		}


		/* Clear screen */
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);

		/* Render text */
		promptText.render((SCREEN_WIDTH - promptText.width()) / 2 , 0);
		for (int i = 0; i < recordingDeviceCount; i++) {
			deviceText[i].render((SCREEN_WIDTH - deviceText[i].width()) / 2,
				promptText.height() + i * deviceText[i].height());
		}

		/* Update screen */
		SDL_RenderPresent(renderer);
	}

	/* Disable text input */
	SDL_StopTextInput();
	
	/* Clean up */
	close();
	return 0;
}


bool init()
{
	bool success = true;

	/* Initialize video */
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		printf("SDL couldn't initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}

	/* Create window */
	window = SDL_CreateWindow("Recordings", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		printf("Window couldn't be created! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}

	/* Create VSync renderer for window */
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL) {
		printf("Renderer couldn't be created! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}

	/* Initialize renderer color */
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

	/* Initialize PNG load */
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		printf("SDL_image coudln't initialize! SDL_image_Error: %s\n", IMG_GetError());
		success = false;
	}

	if (TTF_Init() == -1) {
		printf("Couldn't initalize SDL_TTF!\n");
		success = false;
	}

	/* Set texture filtering to linear */
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		printf("Warning: Linear texture filtering not enabled!\n");

	return true;
}

bool loadMedia()
{
	bool success = true;

	font = TTF_OpenFont("Fonts/lazy.ttf", 28);
	if (font == nullptr) {
		printf("Couldn't open the lazy font!\n");
		success = false;
	}
	else {
		/* Load the starting prompt */
		if (!promptText.loadFromRenderedText("Select your recording device: ", textColor)) {
			printf("Couldn't render starting prompt text!\n");
			success = false;
		}

		/* Get capture device count */
		recordingDeviceCount = SDL_GetNumAudioDevices(SDL_TRUE);

		/* No recording devices */
		if (recordingDeviceCount < 1) {
			printf("Couldn't find audio capture device! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		/* At least one device*/
		else {
			/* Cap the number of devices */
			if (recordingDeviceCount > MAX_RECORDING_DEVICES)
				recordingDeviceCount = MAX_RECORDING_DEVICES;

			/* Load device names */
			std::stringstream text;
			for (int i = 0; i < recordingDeviceCount; ++i) {
				/* Get capture device name */
				text.str("");
				text << i << ": " << SDL_GetAudioDeviceName(i, SDL_TRUE);

				/* Set texture from name */
				deviceText[i].loadFromRenderedText(text.str(), textColor);
			}
		}
	}

	return success;
}

void close()
{
	/* Free textures */
	promptText.free();
	for (int i = 0; i < MAX_RECORDING_DEVICES; ++i)
		deviceText[i].free();

	/* Close the font */
	TTF_CloseFont(font);
	font = nullptr;

	/* Free playback audio */
	if (recordingBuffer != nullptr) {
		delete[] recordingBuffer;
		recordingBuffer = nullptr;
	}

	/* Destroy window */
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	window = nullptr;
	renderer = nullptr;

	/* Quit the SDL library */
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
} 


void audioRecordingCallback(void* userData, Uint8* stream, int len)
{
	/* Copy audio from stream */
	memcpy(&recordingBuffer[bufferBytePosition], stream, len);

	/* Move along buffer */
	bufferBytePosition += len;
}

void audioPlaybackCallback(void* userData, Uint8* stream, int len)
{
	/* Copy audio to stream */
	memcpy(stream, &recordingBuffer[bufferBytePosition], len);

	/* Move along buffer */
	bufferBytePosition += len;
}