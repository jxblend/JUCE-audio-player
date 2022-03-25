#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class WaveformDisplay : public Component,
    public ChangeListener
{
public:
    WaveformDisplay(AudioFormatManager& formatManagerToUse,
        AudioThumbnailCache& cacheToUse);
    ~WaveformDisplay();

    /** fills the component's background */
    void paint(Graphics&) override;

    /** set bounds of child components */
    void resized() override;

    /** updates when changeListener is triggered */
    void changeListenerCallback(ChangeBroadcaster* source) override;

    /** loads URL of song */
    void loadURL(URL audioURL);

    /** set the relative position of the playhead*/
    void setPositionRelative(double pos);

    bool fileLoaded;

private:
    AudioThumbnail audioThumb;
    double position;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformDisplay)
};

