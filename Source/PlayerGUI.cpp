#include "PlayerGUI.h"


PlayerGUI::PlayerGUI()
{
	// Add buttons
	for (auto* btn : { &loadButton, &playPauseButton, &goToStartButton ,&goToEndButton, &loopButton, &muteButton, &abLoopButton })
	{
		btn->addListener(this);
		addAndMakeVisible(btn);
	}

	// Text labels
	title.setColour(juce::Label::textColourId, juce::Colours::black);
	artist.setColour(juce::Label::textColourId, juce::Colours::black);
	duration.setColour(juce::Label::textColourId, juce::Colours::black);
	addAndMakeVisible(title);
	addAndMakeVisible(artist);
	addAndMakeVisible(duration);

	// Volume slider
	volumeSlider.setRange(0.0, 1.0, 0.01);
	volumeSlider.setValue(0.5);
	volumeSlider.addListener(this);
	addAndMakeVisible(volumeSlider);
	// PlayBack Speed Slider
	speedSlider.setRange(0.1, 2.0, 0.01);
	speedSlider.setValue(1.0);
	speedSlider.addListener(this);
	addAndMakeVisible(speedSlider);
	//position slider
	positionSlider.setRange(0.0, 1 ,0.01);
	positionSlider.setValue(0.0);
	positionSlider.addListener(this);
	addAndMakeVisible(positionSlider);
	positionSlider.textFromValueFunction = [](double value) {
		int hours = (int)value / 3600, minutes = ((int)value % 3600)/60, seconds = (int)value % 60;
		if (hours > 0) return juce::String::formatted("%d:%02d:%02d", hours, minutes, seconds);
		 else return juce::String::formatted("%d:%02d", minutes, seconds);
	};
	startTimer(500); // calls timerCallback every 500 milliseconds
	//abLoop Slider
	abLoopSlider.setRange(0.0, 1);
	abLoopSlider.setSliderStyle(juce::Slider::TwoValueHorizontal);
	abLoopSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
	addAndMakeVisible(abLoopSlider);
	abLoopSlider.setVisible(false);
	abLoopSlider.addListener(this);
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
	abLoopButton.setBounds(660, y, 100, 40);
	title.setBounds(20, 80, 200, 30);
	artist.setBounds(240, 80, 200, 30);
	duration.setBounds(410, 80, 200, 30);
	volumeSlider.setBounds(20, 130, getWidth() - 40, 30);
	speedSlider.setBounds(20, 180, getWidth() - 40, 30);
	positionSlider.setBounds(20, 230, getWidth() - 40, 30);
	abLoopSlider.setBounds(100, 280, getWidth() - 120, 30);
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
				if (playerAudio.loadFile(files[0]))
				{
					double length = playerAudio.getLength();
					if (length > 0.0)
						positionSlider.setRange(0.0, length, 0.01); // REAL seconds range
				}
				if (files.size() > 0 && files[0].existsAsFile())
				{
					playerAudio.loadFile(files[0]);

					auto metadata = playerAudio.metaData(fileChooser->getResult());
					title.setText("Title: " + playerAudio.metaData(fileChooser->getResult())[0], juce::dontSendNotification);
					artist.setText("Artist: " + playerAudio.metaData(fileChooser->getResult())[1], juce::dontSendNotification);
					duration.setText("Duration: " + playerAudio.metaData(fileChooser->getResult())[2], juce::dontSendNotification);
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
		if(abLoopButton.getToggleState()){
			abLoopButton.setToggleState(false, juce::dontSendNotification);
			abLoopButton.setButtonText("A-B Loop: Off");
			abLoopSlider.setVisible(false);
			playerAudio.clearabLoop();
		}
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
	else if(button == &abLoopButton) {
		if (loopButton.getToggleState()) {
			loopButton.setToggleState(false, juce::dontSendNotification);
			loopButton.setButtonText("Loop: Off");
			playerAudio.setLooping(false);
		}
		bool abLoop = !abLoopButton.getToggleState();
		abLoopButton.setToggleState(abLoop, juce::dontSendNotification);
		abLoopButton.setButtonText(abLoop ? "A-B Loop: On" : "A-B Loop: Off");
		abLoopSlider.setVisible(abLoop);
		if (abLoop) {
			abLoopSlider.setRange(0.0, playerAudio.getLength(),0.01);
			abLoopSlider.setMinAndMaxValues(0.0, playerAudio.getLength(), juce::dontSendNotification);
			playerAudio.setabLoop(0.0, playerAudio.getLength());
		}
		else playerAudio.clearabLoop();
	}
}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
	if (slider == &volumeSlider)
	{
		playerAudio.setGain(slider->getValue());
	}
	else if (slider == &speedSlider) {
			playerAudio.setSpeed(slider->getValue());
	}
	else if (slider == &positionSlider)
	{
		if(slider->isMouseButtonDown())
			playerAudio.setPosition(slider->getValue());
	}
	else if (slider == &abLoopSlider) {
		playerAudio.setabLoop(abLoopSlider.getMinValue(), abLoopSlider.getMaxValue());
	}
}

void PlayerGUI::timerCallback()
{
	if (!positionSlider.isMouseButtonDown()) {
		return positionSlider.setValue(playerAudio.getRelativePos()*playerAudio.getLength(), juce::dontSendNotification);
	}
}
