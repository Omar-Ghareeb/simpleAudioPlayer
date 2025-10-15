#include "PlayerGUI.h"


PlayerGUI::PlayerGUI()
{
	// Add buttons
	for (auto* btn : { &loadButton, &playButton, &stopButton, &loopButton })
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

PlayerGUI::~PlayerGUI(){}

void PlayerGUI::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	playerAudio.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerGUI::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
	playerAudio.getNextAudioBlock(bufferToFill);
	if(playerAudio.getPosition() >= playerAudio.getLength() && loopButton.getToggleState())
	{
		playerAudio.setPosition(0.0);
		playerAudio.play();
	}
}

void PlayerGUI::releaseResources()
{
	playerAudio.releaseResources();
}

void PlayerGUI::resized()
{
	int y = 20;
	loadButton.setBounds(20, y, 100, 40);
	playButton.setBounds(140, y, 80, 40);
	stopButton.setBounds(240, y, 80, 40);
	loopButton.setBounds(340, y, 100, 40);
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
	else if (button == &playButton)
	{
		playerAudio.play();
	}
	else if (button == &stopButton)
	{
		playerAudio.stop();
	}
	else if(button == &loopButton)
	{
		bool isLooping = !loopButton.getToggleState();
		loopButton.setToggleState(isLooping,juce::dontSendNotification);
		loopButton.setButtonText(isLooping ? "Loop: On" : "Loop: Off");
	}
}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
	if (slider == &volumeSlider)
	{
		playerAudio.setGain(slider->getValue());
	}
}

