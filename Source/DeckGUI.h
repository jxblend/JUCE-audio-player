#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "PlaylistComponent.h"


//==============================================================================
/*
*/
class DeckGUI : public Component,
    public Button::Listener,
    public Slider::Listener,
    public FileDragAndDropTarget,
    public Timer
  
{
public:
    DeckGUI(DJAudioPlayer* player,
        AudioFormatManager& formatManagerToUse,
        AudioThumbnailCache& cacheToUse);
    ~DeckGUI();

    /** fills the component's background and draws text */
    void paint(Graphics&) override;

    /** set bounds of child components */
    void resized() override;

    /** implement Button::Listener */
    void buttonClicked(Button*) override;

    /** implement Slider::Listener */
    void sliderValueChanged(Slider* slider) override;

    /** detects file drag motion over corresponding area */
    bool isInterestedInFileDrag(const StringArray& files) override;

    /** when file released into corresponding area after drag  */
    void filesDropped(const StringArray& files, int x, int y) override;

    /** called at given intervals */
    void timerCallback() override;

private:
    TextButton playButton{ "PLAY" };
    TextButton pauseButton{ "PAUSE" };
    TextButton loadButton{ "LOAD" };
    TextButton loopButton{ "LOOP" };
    TextButton stopLoopButton{ "STOP LOOP" };

    std::string songPlayed;

    Slider volSlider;
    Slider speedSlider;
    Slider posSlider;

    WaveformDisplay waveformDisplay;

    DJAudioPlayer* player;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeckGUI)
};
