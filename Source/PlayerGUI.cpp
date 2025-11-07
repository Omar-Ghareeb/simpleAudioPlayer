#include "PlayerGUI.h"


PlayerGUI::PlayerGUI()
//waveform change detector
		: thumbnailCache(7),
		thumbnail(512, playerAudio.getForamt(), thumbnailCache)
{
	// Add buttons
	juce::TextButton* buttons[] = {
		&goToStartButton, &goToEndButton, &abLoopButton, &addToPlaylistButton, &removeFromPlaylistButton,& reverbButton
	};
	juce::ImageButton* imgButtons[] = { &loadButton, &playPauseButton, &loopButton,
		&muteButton,& forward10Button,& rewind10Button,& nextButton,& previousButton,& addMarker };
	for (auto* btn : buttons)
	{
		btn->addListener(this);
		addAndMakeVisible(btn);
		btn->setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xffa020f0));
		btn->setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
	}
	for (auto* imgBtn : imgButtons)
	{
		imgBtn->addListener(this);
		addAndMakeVisible(imgBtn);
	}

	// Load button image
	loadImage = juce::ImageCache::getFromMemory(BinaryData::fileimport_png, BinaryData::fileimport_pngSize);
	loadButton.setImages(false, true, true, // don�t resize automatically, keep png transparent, allow tint colours on hover/click
		loadImage, 1.0f, juce::Colours::darkgrey, //normal
		loadImage, 1.0f, juce::Colour(0xffa020f0).withAlpha(0.5f), //hover
		loadImage, 1.0f, juce::Colour(0xffa020f0)); //clicked

	// PlayPause button images
	playImage = juce::ImageCache::getFromMemory(BinaryData::play_png, BinaryData::play_pngSize);
	pauseImage = juce::ImageCache::getFromMemory(BinaryData::pause_png, BinaryData::pause_pngSize);
	playPauseButton.setImages(false, true, true, 
		playImage, 1.0f, juce::Colours::darkgrey,
		playImage, 1.0f, juce::Colour(0xffa020f0).withAlpha(0.5f), 
		pauseImage, 1.0f, juce::Colour(0xffa020f0));

	// Loop button image
	loopImage = juce::ImageCache::getFromMemory(BinaryData::loopsquare_png, BinaryData::loopsquare_pngSize);
	loopButton.setImages(false, true, true,
		loopImage, 1.0f, juce::Colours::darkgrey,
		loopImage, 1.0f, juce::Colour(0xffa020f0).withAlpha(0.5f),
		loopImage, 1.0f, juce::Colour(0xffa020f0));

	// Mute button image
	muteImage = juce::ImageCache::getFromMemory(BinaryData::volumemute_png, BinaryData::volumemute_pngSize);
	muteButton.setImages(false, true, true,
		juce::ImageCache::getFromMemory(BinaryData::volume_png, BinaryData::volume_pngSize), 1.0f, juce::Colours::darkgrey,
		muteImage, 1.0f, juce::Colour(0xffa020f0).withAlpha(0.5f),
		muteImage, 1.0f, juce::Colour(0xffa020f0));
	// Forward10 and Rewind10 button image
	forward10Image = juce::ImageCache::getFromMemory(BinaryData::forward_png, BinaryData::forward_pngSize);
	forward10Button.setImages(false, true, true,
		forward10Image, 1.0f, juce::Colours::darkgrey,
		forward10Image, 1.0f, juce::Colour(0xffa020f0).withAlpha(0.5f),
		forward10Image, 1.0f, juce::Colour(0xffa020f0));
	rewind10Image = juce::ImageCache::getFromMemory(BinaryData::rewind_png, BinaryData::rewind_pngSize);
	rewind10Button.setImages(false, true, true,
		rewind10Image, 1.0f, juce::Colours::darkgrey,
		rewind10Image, 1.0f, juce::Colour(0xffa020f0).withAlpha(0.5f),
		rewind10Image, 1.0f, juce::Colour(0xffa020f0));

	// Next and Previous button images
	nextImage = juce::ImageCache::getFromMemory(BinaryData::stepforward_png, BinaryData::stepforward_pngSize);
	nextButton.setImages(false, true, true,
		nextImage, 1.0f, juce::Colours::darkgrey,
		nextImage, 1.0f, juce::Colour(0xffa020f0).withAlpha(0.5f),
		nextImage, 1.0f, juce::Colour(0xffa020f0));
	previousImage = juce::ImageCache::getFromMemory(BinaryData::stepbackward_png, BinaryData::stepbackward_pngSize);
	previousButton.setImages(false, true, true,
		previousImage, 1.0f, juce::Colours::darkgrey,
		previousImage, 1.0f, juce::Colour(0xffa020f0).withAlpha(0.5f),
		previousImage, 1.0f, juce::Colour(0xffa020f0));

	// Add marker images
	addMarkerImage = juce::ImageCache::getFromMemory(BinaryData::mappin_png, BinaryData::mappin_pngSize);
	addMarker.setImages(false, true, true,
		addMarkerImage, 1.0f, juce::Colours::darkgrey,
		addMarkerImage, 1.0f, juce::Colour(0xffa020f0).withAlpha(0.5f),
		addMarkerImage, 1.0f, juce::Colour(0xffa020f0));

	// Text labels
	title.setColour(juce::Label::textColourId, juce::Colours::white);
	artist.setColour(juce::Label::textColourId, juce::Colours::white);
	duration.setColour(juce::Label::textColourId, juce::Colours::white);
	addAndMakeVisible(title);
	addAndMakeVisible(artist);
	addAndMakeVisible(duration);
	// revrb button
	reverbButton.setButtonText("Reverb");
	// Volume slider
	volumeSlider.setRange(0.0, 1.0, 0.01);
	volumeSlider.setValue(0.5);
	volumeSlider.addListener(this);
	volumeSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
	volumeSlider.setColour(juce::Slider::thumbColourId, juce::Colour(0xffa020f0)); // purple dot
	volumeSlider.setColour(juce::Slider::trackColourId, juce::Colour(0xffa020f0));   // track
	volumeSlider.setColour(juce::Slider::backgroundColourId, juce::Colours::darkgrey); // background
	addAndMakeVisible(volumeSlider);
	volumeIcon = juce::Drawable::createFromImageData(BinaryData::volume_svg, BinaryData::volume_svgSize);
	volumeIcon->replaceColour(juce::Colours::black, juce::Colours::darkgrey);

	// PlayBack Speed Slider
	speedSlider.setRange(0.1, 2.0, 0.01);
	speedSlider.setValue(1.0);
	speedSlider.addListener(this);
	speedSlider.setTextBoxStyle(juce::Slider::NoTextBox,false,0,0);
	speedSlider.setColour(juce::Slider::thumbColourId, juce::Colour(0xffa020f0));
	speedSlider.setColour(juce::Slider::trackColourId, juce::Colour(0xffa020f0)); 
	speedSlider.setColour(juce::Slider::backgroundColourId, juce::Colours::darkgrey);
	speedIcon = juce::Drawable::createFromImageData(BinaryData::tachometerfast_svg, BinaryData::tachometerfast_svgSize);
	speedIcon->replaceColour(juce::Colours::black, juce::Colours::darkgrey);
	addAndMakeVisible(speedSlider);

	//position slider
	positionSlider.setRange(0.0, 1 ,0.01);
	positionSlider.setValue(0.0);
	positionSlider.addListener(this);
	positionSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
	positionSlider.setColour(juce::Slider::thumbColourId, juce::Colour(0xffa020f0));
	positionSlider.setColour(juce::Slider::trackColourId, juce::Colour(0xffa020f0));
	positionSlider.setColour(juce::Slider::backgroundColourId, juce::Colours::darkgrey);
	addAndMakeVisible(positionSlider);
	positionSlider.textFromValueFunction = [](double value) {
		int hours = (int)value / 3600, minutes = ((int)value % 3600)/60, seconds = (int)value % 60;
		if (hours > 0) return juce::String::formatted("%d:%02d:%02d", hours, minutes, seconds);
		 else return juce::String::formatted("%d:%02d", minutes, seconds);
	};
	
	//abLoop Slider
	abLoopSlider.setRange(0.0, 1);
	abLoopSlider.setSliderStyle(juce::Slider::TwoValueHorizontal);
	abLoopSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
	addAndMakeVisible(abLoopSlider);
	abLoopSlider.setColour(juce::Slider::thumbColourId, juce::Colour(0xffa020f0));
	abLoopSlider.setColour(juce::Slider::trackColourId, juce::Colour(0xffa020f0));
	abLoopSlider.setColour(juce::Slider::backgroundColourId, juce::Colours::darkgrey);
	abLoopSlider.setVisible(false);
	abLoopSlider.addListener(this);

	// Playlist Table
	addAndMakeVisible(playListTable);
	playListTable.setModel(this);
	playListTable.setColour(juce::ListBox::backgroundColourId, juce::Colours::darkgrey.withAlpha(0.5f));
	// markers comboBox 
	addAndMakeVisible(Markers);
	Markers.addListener(this);
	Markers.setText("select your marker");
	//waveform
	thumbnail.addChangeListener(this);
	startTimer(40); // calls timerCallback every 40 milliseconds

	//load session file settings
	juce::PropertiesFile::Options options;
	options.applicationName = "AudioPlayerApp";
	options.filenameSuffix = ".settings";
	options.folderName = "AudioPlayerAppData";
	propertiesFile = std::make_unique<juce::PropertiesFile>(options);
	
}

PlayerGUI::~PlayerGUI()
{
	
}

void PlayerGUI::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	playerAudio.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerGUI::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
	playerAudio.getNextAudioBlock(bufferToFill);
	currentFile = playerAudio.getCurrentFile();
}

void PlayerGUI::releaseResources()
{
	playerAudio.releaseResources();
}

void PlayerGUI::resized()
{
	loadButton.setBoundsRelative(0.9f, 0.8f, 0.07f, 0.07f);
	playPauseButton.setBoundsRelative(0.59f, 0.79f, 0.08f, 0.08f);
	goToStartButton.setBoundsRelative(0.55f, 0.05f, 0.06f, 0.06f);
	goToEndButton.setBoundsRelative(0.62f, 0.05f, 0.06f, 0.06f);
	loopButton.setBoundsRelative(0.85f, 0.8f, 0.06f, 0.06f);
	muteButton.setBoundsRelative(0.79f, 0.8f, 0.06f, 0.06f);
	reverbButton.setBoundsRelative(0.79f, 0.05f, 0.06f, 0.06f);
	abLoopButton.setBoundsRelative(0.7f, 0.05f, 0.08f, 0.06f);
	forward10Button.setBoundsRelative(0.69f, 0.8f, 0.06f, 0.06f);
	rewind10Button.setBoundsRelative(0.53f, 0.8f, 0.06f, 0.06f);
	title.setBoundsRelative(0.3f, 0.03f, 0.2f, 0.1f);
	artist.setBoundsRelative(0.3, 0.11f, 0.2f, 0.07f);
	duration.setBoundsRelative(0.3f, 0.15f, 0.2f, 0.07f);
	volumeSlider.setBoundsRelative(0.32f, 0.78f, 0.1f, 0.1f);
	auto volumeArea = getLocalBounds().getProportion(
		juce::Rectangle<float>(0.29f, 0.8f, 0.03f, 0.06f));
	//juce::Rectangle<float> volumeArea(20, 130, 30, 30);
	if (volumeIcon)
		volumeIcon->setTransformToFit(volumeArea.toFloat(), juce::RectanglePlacement::centred);
	speedSlider.setBoundsRelative(0.43f, 0.78f, 0.1f, 0.1f);
	auto speedArea = getLocalBounds().getProportion(
		juce::Rectangle<float>(0.41f, 0.8f, 0.03f, 0.06f));
	//juce::Rectangle<float> speedArea(20, 180, 30, 30);
	if (speedIcon)
		speedIcon->setTransformToFit(speedArea.toFloat(), juce::RectanglePlacement::centred);
	positionSlider.setBoundsRelative(0.33f, 0.7f, 0.6f, 0.1f);
	abLoopSlider.setBoundsRelative(0.33f, 0.3f, 0.6f, 0.1f);
	addToPlaylistButton.setBoundsRelative(0.01f, 0.01f, 0.07f, 0.07f);
	removeFromPlaylistButton.setBoundsRelative(0.09f, 0.01f, 0.07f, 0.07f);
	nextButton.setBoundsRelative(0.25f, 0.02f, 0.03f, 0.05f);
	previousButton.setBoundsRelative(0.17f, 0.02f, 0.03f, 0.05f);
	playListTable.setBoundsRelative(0.0f, 0.08f, 0.28f, 1.0f);
	addMarker.setBoundsRelative(0.9f, 0.04f, 0.04f, 0.04f);
	Markers.setBoundsRelative(0.9f, 0.08f, 0.04f, 0.04f);
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
					playerAudio.clearMarkers();
					Markers.clear();
					thumbnail.setSource(new juce::FileInputSource(files[0]));

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
			playPauseButton.setToggleState(isPlaying, juce::dontSendNotification);
		}
		else
		{
			playerAudio.pause();
			playPauseButton.setToggleState(isPlaying, juce::dontSendNotification);
		}
	}
	else if (button == &goToStartButton)
	{
		playerAudio.goToStart();
		playerAudio.play();
		bool isPlaying = !playPauseButton.getToggleState();
		if (isPlaying) {
				playPauseButton.setToggleState(isPlaying, juce::dontSendNotification);
		}

	}
	else if (button == &goToEndButton)
	{
		playerAudio.goToEnd();
			playerAudio.pause();
			bool isPlaying = !playPauseButton.getToggleState();
			if (!isPlaying) {
					playPauseButton.setToggleState(isPlaying, juce::dontSendNotification);
			}
		
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
		playerAudio.setLooping(loop);
	}
	else if (button == &muteButton) {
		bool muted = !muteButton.getToggleState();
		muteButton.setToggleState(muted, juce::dontSendNotification);
		if (muted) {
			volumeSlider.setValue(0.0, juce::dontSendNotification);
		}
		else {
			volumeSlider.setValue(0.1, juce::dontSendNotification);
		}
		playerAudio.mute();
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

	else if (button == &addToPlaylistButton) {
		fileChooser = std::make_unique<juce::FileChooser>("Select audio files to add to playlist...",
			juce::File{},
			"*.wav;*.mp3;*.aiff");
		fileChooser->launchAsync(juce::FileBrowserComponent::openMode
			| juce::FileBrowserComponent::canSelectFiles
			| juce::FileBrowserComponent::canSelectMultipleItems,
			[this](const juce::FileChooser& fc)
			{
				auto files = fc.getResults();
				for (const auto& file : files) {
					if (file.existsAsFile()) {
						playerAudio.addToPlayList(file);
						playListTable.updateContent();
					}
				}
			});
	}
	else if (button == &removeFromPlaylistButton) {
		int selectedRow = playListTable.getSelectedRow();
		if (selectedRow >= 0) {
		
			playerAudio.unloadFile();
			playerAudio.removeFromPlayList(selectedRow);
			playListTable.updateContent();
			title.setText("", juce::dontSendNotification);
			artist.setText("", juce::dontSendNotification);
			duration.setText("", juce::dontSendNotification);
			thumbnail.clear();      
			repaint();
		}
	}

	else if (button == &nextButton) {
			playerAudio.nextTrack();
			thumbnail.setSource(new juce::FileInputSource(playerAudio.getCurrentFile()));
			repaint();
			int currentIndex = playerAudio.findIndex();
			if (currentIndex >= 0) {
					playerAudio.setIndex(currentIndex);
					playListTable.selectRow(currentIndex);
					playListTable.updateContent();
					playListTable.repaint();
			}
			auto metadata = playerAudio.metaData(playerAudio.getCurrentFile());
			title.setText("Title: " + metadata[0], juce::dontSendNotification);
			artist.setText("Artist: " + metadata[1], juce::dontSendNotification);
			duration.setText("Duration: " + metadata[2], juce::dontSendNotification);
			bool isPlaying = !playPauseButton.getToggleState();
			if (isPlaying) {
					playPauseButton.setToggleState(isPlaying, juce::dontSendNotification);
			}
			}

	else if (button == &previousButton) {
			playerAudio.previousTrack();
			thumbnail.setSource(new juce::FileInputSource(playerAudio.getCurrentFile()));
			repaint();
			int currentIndex = playerAudio.findIndex();
			if (currentIndex >= 0) {
					playerAudio.setIndex(currentIndex);
					playListTable.selectRow(currentIndex);
					playListTable.updateContent();
					playListTable.repaint();
			}
			auto metadata = playerAudio.metaData(playerAudio.getCurrentFile());
			title.setText("Title: " + metadata[0], juce::dontSendNotification);
			artist.setText("Artist: " + metadata[1], juce::dontSendNotification);
			duration.setText("Duration: " + metadata[2], juce::dontSendNotification);
			bool isPlaying = !playPauseButton.getToggleState();
			if (isPlaying) {
					playPauseButton.setToggleState(isPlaying, juce::dontSendNotification);
			}
			}

	else if (button == &forward10Button) {
		playerAudio.forward10Seconds();
		}

	else if (button == &rewind10Button) {
			playerAudio.rewind10Seconds();
			}
	else if (button == &addMarker) {
			int hours = (int)playerAudio.getPosition() / 3600, minutes = ((int)playerAudio.getPosition() % 3600) / 60, seconds = (int)playerAudio.getPosition() % 60;
			juce::String pos;
			if (hours > 0) pos= juce::String::formatted("%d:%02d:%02d", hours, minutes, seconds);
			else pos = juce::String::formatted("%d:%02d", minutes, seconds);
					Markers.addItem(pos, playerAudio.getSize() + 1);
					playerAudio.addMarker();
					}
	else if (button == &reverbButton) {
			bool state = !reverbButton.getToggleState();
			reverbButton.setToggleState(state, juce::dontSendNotification);
			playerAudio.reverbOn(state);
	}
   
}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
	if (slider == &volumeSlider)
	{
		if (slider->getValue() == 0.0) {
			muteButton.setToggleState(true, juce::dontSendNotification);
		}
		else {
			muteButton.setToggleState(false, juce::dontSendNotification);
		}
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
				positionSlider.setValue(playerAudio.getRelativePos() * playerAudio.getLength(), juce::dontSendNotification);
		}

		repaint();
}

int PlayerGUI::getNumRows()
{
	int size = playerAudio.getPlayListSize();
	return size > 0 ? size : 0;
}

void PlayerGUI::paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected)
{
	if (!(rowNumber < 0 || rowNumber >= playerAudio.getPlayListSize())) {
		g.setColour(juce::Colours::silver);
		if (rowIsSelected) {
			g.fillAll(juce::Colour(0xffa020f0));
			g.setColour(juce::Colours::white);
		}
		auto metadata = playerAudio.metaData(playerAudio.getPlayListFileAt(rowNumber));
		juce::String displayInfo = juce::String(rowNumber + 1) + ". " + metadata[0];
		g.drawText(displayInfo, 4, 0, width - 4, height, juce::Justification::centredLeft);

	}
}

void PlayerGUI::listBoxItemClicked(int row, const juce::MouseEvent& e)
{
	if (row >= 0 && row < playerAudio.getPlayListSize()) {
		playerAudio.loadFile(playerAudio.getPlayListFileAt(row));
		auto metadata = playerAudio.metaData(playerAudio.getPlayListFileAt(row));
		title.setText("Title: " + juce::String(metadata[0]), juce::dontSendNotification);
		artist.setText("Artist: " + juce::String(metadata[1]), juce::dontSendNotification);
		duration.setText("Duration: " + juce::String(metadata[2]), juce::dontSendNotification);
		playerAudio.play();
		playPauseButton.setButtonText("Pause");
		bool isPlaying = !playPauseButton.getToggleState();
		if (!playPauseButton.getToggleState()) {
				playPauseButton.setToggleState(isPlaying, juce::dontSendNotification);
		}
		playerAudio.clearMarkers();
		Markers.clear();
		double length = playerAudio.getLength();
		if (length > 0.0)
				positionSlider.setRange(0.0, length, 0.01); // REAL seconds range
		thumbnail.setSource(new juce::FileInputSource(playerAudio.getPlayListFileAt(row)));
	}
}
void PlayerGUI::comboBoxChanged(juce::ComboBox* box) {
		if (box = &Markers) {
				if (Markers.getSelectedId() > 0) {
						playerAudio.moveToMarker((Markers.getSelectedId()));
				}
				Markers.setSelectedId(0);


		}
}
void PlayerGUI::changeListenerCallback(juce::ChangeBroadcaster* source)
{
		if (source == &thumbnail) { repaint(); }
}

void PlayerGUI::paint(juce::Graphics& g) {
	g.fillAll(juce::Colour(0xFF121212));
	auto area = getLocalBounds();
	juce::Rectangle<int>  thumbnailBounds = area.getProportion(
		juce::Rectangle<float>(0.34f, 0.4f, 0.58f, 0.2f));
	//juce::Rectangle<int> thumbnailBounds(450, 300, getWidth() - 450, getHeight() - 300);
		if (thumbnail.getNumChannels() == 0) {
				g.setColour(juce::Colours::darkgrey);
				g.fillRect(thumbnailBounds);
				g.setColour(juce::Colours::red);
				g.drawFittedText("No File Loaded", thumbnailBounds, juce::Justification::centred, 1);
		}
		else {
				g.setColour(juce::Colours::black);
				g.fillRect(thumbnailBounds);
				g.setColour(juce::Colour(0xffa020f0));
				thumbnail.drawChannel(g, thumbnailBounds, 0.0, thumbnail.getTotalLength(),0, 1.0f);
				thumbnail.drawChannel(g, thumbnailBounds, 0.0, thumbnail.getTotalLength(), 1, 1.0f);

				double positionInSec = playerAudio.getPosition();
				double thumbnailLength = thumbnail.getTotalLength();
				float relativePos = (float)positionInSec / thumbnailLength;
				float lineX = thumbnailBounds.getX() + relativePos * thumbnailBounds.getWidth();
				g.setColour(juce::Colours::white);
				g.drawLine(lineX, (float)thumbnailBounds.getY(), lineX, (float)thumbnailBounds.getBottom(), 2.0f);
				//int songPtr = int((getWidth() - 450) * (positionInSec / thumbnailLength)) + 450;
				//g.drawLine((float)songPtr, float(getHeight()), (float)songPtr, float(getHeight() - 30), 2.0f);
		}


		
		g.setColour(juce::Colours::darkgrey);

		if (volumeIcon)
			volumeIcon->draw(g, 1.0f);
		if (speedIcon)
			speedIcon->draw(g, 1.0f);
}

void PlayerGUI::saveSession(const juce::String& playerId)
{
		if (playerAudio.getCurrentFilePath() != "") {
				propertiesFile->setValue(playerId + "_filePath", playerAudio.getCurrentFilePath());
				propertiesFile->setValue(playerId + "_positionSeconds", playerAudio.getPosition());
				propertiesFile->setValue(playerId + "_length", playerAudio.getLength());
		}
		propertiesFile->saveIfNeeded();
}

void PlayerGUI::loadSession(const juce::String& playerId)
{
		auto filePath = propertiesFile->getValue(playerId + "_filePath", "");
		juce::File file(filePath);

		if (file.existsAsFile())
		{
				playerAudio.loadFile(file);
				playerAudio.clearMarkers();
				Markers.clear();
				thumbnail.setSource(new juce::FileInputSource(file));

				auto metadata = playerAudio.metaData(file);
				title.setText("Title: " + metadata[0], juce::dontSendNotification);
				artist.setText("Artist: " + metadata[1], juce::dontSendNotification);
				duration.setText("Duration: " + metadata[2], juce::dontSendNotification);
		}
		double length = propertiesFile->getDoubleValue(playerId + "_length", 1);
		if (length > 0.0)
				positionSlider.setRange(0.0, length, 0.01);
		double pos = propertiesFile->getDoubleValue(playerId + "_positionSeconds", 0.0);
		if (pos > 0.0 && pos < playerAudio.getLength())
				playerAudio.setPosition(pos);
		positionSlider.setValue(pos, juce::dontSendNotification);
}