#include "PlayerGUI.h"


PlayerGUI::PlayerGUI()
{
	// Add buttons
	for (auto* btn : { &loadButton, &playPauseButton, &goToStartButton ,&goToEndButton, &loopButton, &muteButton })
	{
		btn->addListener(this);
		addAndMakeVisible(btn);
	}
	// Volume slider
	volumeSlider.setRange(0.0, 1.0, 0.01);
	volumeSlider.setValue(0.5);
	volumeSlider.addListener(this);
	addAndMakeVisible(volumeSlider);
}

PlayerGUI::~PlayerGUI() {}

void PlayerGUI::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	playerAudio.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerGUI::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
	playerAudio.getNextAudioBlock(bufferToFill);
}

void PlayerGUI::releaseResources()
{
	playerAudio.releaseResources();
}

void PlayerGUI::resized()
{
	int y = 20;
	loadButton.setBounds(20, y, 100, 40);
	playPauseButton.setBounds(140, y, 80, 40);
	goToStartButton.setBounds(240, y, 80, 40);
	goToEndButton.setBounds(340, y, 80, 40);
	loopButton.setBounds(440, y, 100, 40);
	muteButton.setBounds(560, y, 80, 40);
	volumeSlider.setBounds(20, 100, getWidth() - 40, 30);
}

void PlayerGUI::buttonClicked(juce::Button* button)
{
	if (button == &loadButton)
	{
		fileChooser = std::make_unique<juce::FileChooser>("Select audio files to play...",
			juce::File{},
			"*.wav;*.mp3;*.aiff");

		fileChooser->launchAsync(juce::FileBrowserComponent::openMode
			| juce::FileBrowserComponent::canSelectFiles,
			[this](const juce::FileChooser& fc)
			{
				auto files = fc.getResults();
				if (files.size() > 0 && files[0].existsAsFile())
				{
					playerAudio.loadFile(files[0]);
				}
			});
	}
	else if (button == &playPauseButton)
	{
		bool isPlaying = !playPauseButton.getToggleState();
		if (isPlaying)
		{
			playerAudio.play();
			playPauseButton.setButtonText("Pause");
			playPauseButton.setToggleState(isPlaying, juce::dontSendNotification);
		}
		else
		{
			playerAudio.pause();
			playPauseButton.setButtonText("Play");
			playPauseButton.setToggleState(isPlaying, juce::dontSendNotification);
		}
	}
	else if (button == &goToStartButton)
	{
		playerAudio.goToStart();
	}
	else if (button == &goToEndButton)
	{
		playerAudio.goToEnd();
	}
	else if (button == &loopButton)
	{
		bool loop = !loopButton.getToggleState();
		loopButton.setToggleState(loop, juce::dontSendNotification);
		loopButton.setButtonText(loop ? "Loop: On" : "Loop: Off");
		playerAudio.setLooping(loop);
	}
	else if (button == &muteButton) {
			bool muted = !muteButton.getToggleState();
			muteButton.setToggleState(muted, juce::dontSendNotification);
			playerAudio.mute();
			muteButton.setButtonText(muted ? "Muted" : "Mute");
	}
}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
	if (slider == &volumeSlider)
	{
		playerAudio.setGain(slider->getValue());
	}
}

