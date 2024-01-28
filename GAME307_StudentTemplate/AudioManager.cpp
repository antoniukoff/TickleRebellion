#include "AudioManager.h"
#include <iostream>

void SoundEffect::play(int loops)
{
	if (Mix_PlayChannel(-1, m_chunk, loops) == -1) {
		if (Mix_PlayChannel(0, m_chunk, loops)) {
			std::cerr << "Mix_PlayChannel error: " << Mix_GetError() << std::endl;
		}
	}
}
void Music::play(int loops)
{
	Mix_PlayMusic(m_music, loops);
}

void Music::pause()
{
	Mix_PauseMusic();
}

void Music::stop()
{
	Mix_HaltMusic();
}

void Music::resume()
{
	Mix_ResumeMusic();
}

AudioEngine::AudioEngine()
{
	//empty
}
AudioEngine::~AudioEngine()
{
	destroy();
}
void AudioEngine::init()
{
	if (m_isInitialized) {
		std::cerr << "Tried to initialize Audio Engine twice!\n";
	}
	//parameters can be a bitwise combinaiton of MIX_INIT_FAC, 
	//MIX_INIT_MOD, MIX_INIT_MP3, MIX_INIT_OGG
	if (Mix_Init(MIX_INIT_MP3) == -1) {
		std::cerr << "Mix_Init error: " << Mix_GetError() << std::endl;
	}

	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
		std::cerr << "Mix_OpenAudio error: " << Mix_GetError() << std::endl;
	}
	m_isInitialized = true;
}
void AudioEngine::destroy()
{
	if (m_isInitialized) {
		m_isInitialized = false;

		for (auto& it : m_effectMap) {
			Mix_FreeChunk(it.second);
		}
		for (auto& it : m_musicMap) {
			Mix_FreeMusic(it.second);
		}
		m_effectMap.clear();
		m_musicMap.clear();

		Mix_CloseAudio();
		Mix_Quit();
	}
}

SoundEffect AudioEngine::loadSoundEffect(const std::string& filePath)
{
	//try to find the audio in the map
	auto it = m_effectMap.find(filePath);

	SoundEffect effect;

	if (it == m_effectMap.end()) {
		//failed to find if must load the sound to the map
		Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());
		//check for errors
		if (chunk == nullptr) {
			std::cerr << "Mix_LoadWAV error: " << Mix_GetError() << std::endl;
		}
		effect.m_chunk = chunk;
		m_effectMap[filePath] = chunk;

	}
	else {
		effect.m_chunk = it->second;
	}
	return effect;
}
Music AudioEngine::loadMusic(const std::string& filePath)
{
	auto it = m_musicMap.find(filePath);

	Music music;

	if (it == m_musicMap.end()) {
		//failed to find if must load the sound to the map
		Mix_Music* mixMusic = Mix_LoadMUS(filePath.c_str());
		//check for errors
		if (mixMusic == nullptr) {
			std::cerr << "Mix_LoadMUS error: " << Mix_GetError() << std::endl;
		}
		music.m_music = mixMusic;
		m_musicMap[filePath] = mixMusic;

	}
	else {
		music.m_music = it->second;
	}
	return music;
}

