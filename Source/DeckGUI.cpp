#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player,
    AudioFormatManager& formatManagerToUse,
    AudioThumbnailCache& cacheToUse
) : player(_player),
waveformDisplay(formatManagerToUse, cacheToUse)
{

    addAndMakeVisible(playButton);
    addAndMakeVisible(pauseButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(loopButton);
    addAndMakeVisible(stopLoopButton);

    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);

    addAndMakeVisible(waveformDisplay);


    playButton.addListener(this);
    pauseButton.addListener(this);
    loadButton.addListener(this);
    loopButton.addListener(this);
    stopLoopButton.addListener(this);

    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);
 
    volSlider.setSliderStyle(Slider::RotaryHorizontalDrag);
    speedSlider.setSliderStyle(Slider::RotaryHorizontalDrag);
   // volSlider.setRotaryParameters(Slider::RotaryParameters(, float_Pi, false));

    volSlider.setRange(0.0, 1.0);
    volSlider.setValue(0.5);
    volSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 20);
    speedSlider.setRange(0.1, 5.0);
    speedSlider.setValue(1.0);
    speedSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 20);
    posSlider.setRange(0.0, 1.0);
    posSlider.setTextBoxStyle(Slider::NoTextBox,true,0,0);
    posSlider.setColour(Slider::ColourIds::trackColourId, Colour(0xFF04dead));
    posSlider.setColour(Slider::ColourIds::backgroundColourId, Colour(0xFF027387));
    startTimer(500);

}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint(Graphics& g)
{
    g.fillAll(Colour(0xFF003a40));   // clear the background
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component
    getLookAndFeel().setColour(TextButton::buttonColourId, Colours::teal);
    getLookAndFeel().setColour(TextButton::textColourOffId, Colour(0xFF6af5eb));
    getLookAndFeel().setColour(Slider::thumbColourId, Colour(0xFF00f17f));
    getLookAndFeel().setColour(Slider::rotarySliderFillColourId, Colour(0xFF04dead));
    getLookAndFeel().setColour(Slider::rotarySliderOutlineColourId, Colour(0xFF027387));
    g.setColour(Colours::springgreen);
    g.setFont(14.0f);

    if (PlaylistComponent::selectedRowNo > 0)
        {
            g.drawText("Playing: " + songPlayed , (getWidth() / 8) * 3.1, getHeight() / 15, 200, 20,
                Justification::centred, true);
            repaint();
        }
    g.drawText("VOLUME / SPEED", (getWidth()/9) * 4, getHeight() /3, 100, 20,
        Justification::centred, true);  

}

void DeckGUI::resized()
{
    double rowH = getHeight() / 9;
    double buttonHeight = getHeight() / 15;
    double buttonWidthLoc = getWidth() / 7;
    double buttonWidth = getWidth() / 8;
    playButton.setBounds(buttonWidthLoc * 3, rowH * 7.5, buttonWidth, buttonHeight);
    pauseButton.setBounds(buttonWidthLoc * 4, rowH * 7.5, buttonWidth, buttonHeight);
    volSlider.setBounds(0, rowH / 2 , getWidth() / 2, rowH * 3.5);
    speedSlider.setBounds(getWidth() / 2, rowH / 2, getWidth() / 2, rowH * 3.5);
    posSlider.setBounds(0, rowH * 4, getWidth(), rowH);
    waveformDisplay.setBounds(0, rowH * 5, getWidth(), rowH * 1.5 );
    loadButton.setBounds(buttonWidthLoc * 3, rowH * 6.7, buttonWidth, buttonHeight);
    loopButton.setBounds(buttonWidthLoc * 2, rowH * 7.5, buttonWidth, buttonHeight);
    stopLoopButton.setBounds(buttonWidthLoc * 3, rowH * 8.3, buttonWidth, buttonHeight);

}

void DeckGUI::buttonClicked(Button* button)
{
    if (button == &playButton)
    {
        player->start();
    }
    if (button == &pauseButton)
    {      
        player->stop();
    }
    if (button == &loadButton)
    {
        if (PlaylistComponent::selectedRowNo > 0)
        {
            File trackRow = PlaylistComponent::songURL[PlaylistComponent::selectedRowNo - 1];
            URL song = URL{ trackRow };
            DBG(PlaylistComponent::selectedRowNo);
            player->loadURL(song);
            waveformDisplay.loadURL(song);
        }
        songPlayed = PlaylistComponent::trackTitles[PlaylistComponent::selectedRowNo - 1];
    }
    if (button == &loopButton)
    {
        player->setLooping(true);
    }
    if (button == &stopLoopButton)
    {
        player->setLooping(false);
    }

}

void DeckGUI::sliderValueChanged(Slider* slider)
{
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }

    if (slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue());
    }

}

bool DeckGUI::isInterestedInFileDrag(const StringArray& files)
{
    DBG("DeckGUI::isInterestedInFileDrag");
    return true;
}

void DeckGUI::filesDropped(const StringArray& files, int x, int y)
{
    for (File filename : files)
    {
        DBG("DeckGUI::filesDropped" << filename.getFileNameWithoutExtension());
        URL fileURL = URL{ filename };
        player->loadURL(fileURL);
        waveformDisplay.loadURL(fileURL);
        return;
    }
}

void DeckGUI::timerCallback()
{
    waveformDisplay.setPositionRelative(
        player->getPositionRelative());
}












