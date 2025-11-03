#include "PlayerGUI.h"


PlayerGUI::PlayerGUI()
//waveform change detector
		: thumbnailCache(7),
		thumbnail(512, playerAudio.getForamt(), thumbnailCache)
{
	// Add buttons
	juce::TextButton* buttons[] = {
		&goToStartButton, &goToEndButton, &abLoopButton, &addToPlaylistButton, &removeFromPlaylistButton,
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
	loadButton.setImages(false, true, true, // don’t resize automatically, keep png transparent, allow tint colours on hover/click
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
	loadSession();
}

PlayerGUI::~PlayerGUI()
{
	saveSession();
}

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
	forward10Button.setBounds(780, y, 60, 40);
	rewind10Button.setBounds(860, y, 60, 40);
	title.setBounds(20, 80, 200, 30);
	artist.setBounds(240, 80, 200, 30);
	duration.setBounds(410, 80, 200, 30);
	volumeSlider.setBounds(60, 130, 200, 30);
	juce::Rectangle<float> volumeArea(20, 130, 30, 30);
	if (volumeIcon)
		volumeIcon->setTransformToFit(volumeArea, juce::RectanglePlacement::centred);
	speedSlider.setBounds(60, 180, 200, 30);
	juce::Rectangle<float> speedArea(20, 180, 30, 30);
	if (speedIcon)
		speedIcon->setTransformToFit(speedArea, juce::RectanglePlacement::centred);
	positionSlider.setBounds(20, 230, getWidth() - 40, 30);
	abLoopSlider.setBounds(100, 280, getWidth() - 120, 30);
	addToPlaylistButton.setBounds(20, 350, 100, 40);
	removeFromPlaylistButton.setBounds(140, 350, 100, 40);
	nextButton.setBounds(260, 350, 80, 40);
	previousButton.setBounds(360, 350, 80, 40);
	playListTable.setBounds(20, 410, 420, getHeight() - 350);
	addMarker.setBounds(960, y, 80, 40);
	Markers.setBounds(960, y + 30, 80, 40);
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
			playerAudio.removeFromPlayList(selectedRow);
			playListTable.updateContent();
		}
	}

	else if (button == &nextButton) {
		playerAudio.nextTrack();
		int currentIndex = playerAudio.getCurrentPlayListIndex();
		if (currentIndex >= 0) {
			auto metadata = playerAudio.metaData(playerAudio.getCurrentFile());
			title.setText("Title: " + metadata[0], juce::dontSendNotification);
			artist.setText("Artist: " + metadata[1], juce::dontSendNotification);
			duration.setText("Duration: " + metadata[2], juce::dontSendNotification);
			playListTable.selectRow(currentIndex);
			playListTable.updateContent();
			playListTable.repaint();
		}

	}
	else if (button == &previousButton) {
		playerAudio.previousTrack();
		int currentIndex = playerAudio.getCurrentPlayListIndex();
		if (currentIndex >= 0) {
			auto metadata = playerAudio.metaData(playerAudio.getCurrentFile());
			title.setText("Title: " + metadata[0], juce::dontSendNotification);
			artist.setText("Artist: " + metadata[1], juce::dontSendNotification);
			duration.setText("Duration: " + metadata[2], juce::dontSendNotification);
			playListTable.selectRow(currentIndex);
			playListTable.updateContent();
			playListTable.repaint();
		}

	}
	else if (button == &forward10Button) {
		playerAudio.forward10Seconds();
		}

	else if (button == &rewind10Button) {
			playerAudio.rewind10Seconds();
			}
	else if (button == &addMarker) {
					Markers.addItem(std::to_string(playerAudio.getPosition()), playerAudio.getSize() + 1);
					playerAudio.addMarker();
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
		playPauseButton.setToggleState(isPlaying, juce::dontSendNotification);
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
		juce::Rectangle<int> thumbnailBounds(450, 300, getWidth() - 450, getHeight() - 300);
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
				thumbnail.drawChannels(g, thumbnailBounds, 0.0, thumbnail.getTotalLength(), 1.0f);

				double positionInSec = playerAudio.getPosition();
				double thumbnailLength = thumbnail.getTotalLength();
				int songPtr = int((getWidth()-450) * (positionInSec / thumbnailLength))+450;
				g.setColour(juce::Colours::red);
				g.drawLine((float)songPtr, float(getHeight()), (float)songPtr, float(getHeight() - 30), 2.0f);
		}


		
		g.setColour(juce::Colours::darkgrey);

		if (volumeIcon)
			volumeIcon->draw(g, 1.0f);
		if (speedIcon)
			speedIcon->draw(g, 1.0f);
}

void PlayerGUI::saveSession()
{
	if (auto currentFile = playerAudio.getCurrentFile(); currentFile.existsAsFile())
		propertiesFile->setValue("filePath", currentFile.getFullPathName());
	propertiesFile->setValue("positionSeconds", playerAudio.getPosition());
	propertiesFile->setValue("length", playerAudio.getLength());
	propertiesFile->saveIfNeeded();
}

void PlayerGUI::loadSession()
{
	auto filePath = propertiesFile->getValue("filePath","");
	juce::File file(filePath);
	if (file.existsAsFile()) {
		playerAudio.loadFile(file);
		playerAudio.clearMarkers();
		Markers.clear();
		thumbnail.setSource(new juce::FileInputSource(file));
		auto metadata = playerAudio.metaData(file);
		title.setText("Title: " + metadata[0], juce::dontSendNotification);
		artist.setText("Artist: " + metadata[1], juce::dontSendNotification);
		duration.setText("Duration: " + metadata[2], juce::dontSendNotification);
	}
	double length = propertiesFile->getDoubleValue("length",1);
	if (length > 0.0)
		positionSlider.setRange(0.0, length, 0.01);
	double pos = propertiesFile->getDoubleValue("positionSeconds", 0.0);
	if (pos > 0.0 && pos < playerAudio.getLength())
		playerAudio.setPosition(pos);
}