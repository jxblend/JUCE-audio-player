#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class DJAudioPlayer : public AudioSource {
public:

    DJAudioPlayer(AudioFormatManager& _formatManager);
    ~DJAudioPlayer();

    /** receives audio data in preparation of play */
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

    /** get the following audio buffer block */
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;

    /** allows the source to release anything it no longer needs after playback has stopped */
    void releaseResources() override;

    /** loads URL of song */
    void loadURL(URL audioURL);

    /** sets volume */
    void setGain(double gain);

    /** sets speed */
    void setSpeed(double ratio);

    /** sets position on the song */
    void setPosition(double posInSecs);

    /** sets relative position on the song*/
    void setPositionRelative(double pos);

    /** sets song to loop */
    void setLooping(bool shouldLoop);
    
    /** start playing */
    void start();

    /** stop playing */
    void stop();

    /** get the relative position of the playhead */
    double getPositionRelative();

private:
    AudioFormatManager& formatManager;
    std::unique_ptr<AudioFormatReaderSource> readerSource;
    AudioTransportSource transportSource;
    ResamplingAudioSource resampleSource{ &transportSource, false, 2 };

};




