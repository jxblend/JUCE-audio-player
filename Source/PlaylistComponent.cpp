#include "../JuceLibraryCode/JuceHeader.h"
#include "PlaylistComponent.h"

std::vector<std::string> PlaylistComponent::trackTitles;
std::vector<File> PlaylistComponent::songURL;
int PlaylistComponent::selectedRowNo;

PlaylistComponent::PlaylistComponent(AudioFormatManager& _formatManager)
    : formatManager(_formatManager)

{
    trackNumber.push_back(0);
    trackNo = 1;

    tableComponent.getHeader().addColumn("Track number", 1, 50);
    tableComponent.getHeader().addColumn("Track title", 2, 200);
    tableComponent.getHeader().addColumn("Duration", 3, 200);

    tableComponent.getHeader().setStretchToFitActive(true);

    tableComponent.setModel(this);

    addAndMakeVisible(tableComponent);
    addAndMakeVisible(lbl);
    addAndMakeVisible(addButton);

    addButton.addListener(this);

    lbl.setTextToShowWhenEmpty("Search playlist", Colours::antiquewhite);

    lbl.onTextChange = [this]
    {
        int rowCount = 0;
        for (std::string track : trackTitles)
        {
            std::string textTyped = lbl.getText().toStdString();
            std::transform(textTyped.begin(), textTyped.end(), textTyped.begin(), ::tolower);
            std::transform(track.begin(), track.end(), track.begin(), ::tolower);
            if (track.find(textTyped) != std::string::npos) {
                tableComponent.selectRow(rowCount);
                tableComponent.scrollToEnsureRowIsOnscreen(rowCount);
            }
            rowCount = rowCount + 1;
        }
    };

    formatManager.registerBasicFormats();
    std::string line;
    std::ifstream playlistFile("playlist.txt");
    if (playlistFile.is_open())
    {
        while (getline(playlistFile, line))
        { 
            String path = line;
            File id(path);
            insertTracks(id);
        }
        playlistFile.close();
    }
  
}

PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint(juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

   
    getLookAndFeel().setColour(TableListBox::backgroundColourId, Colour(0xFF041f18));
    getLookAndFeel().setColour(TableHeaderComponent::backgroundColourId, Colour(0xFF122a2d));
    getLookAndFeel().setColour(TableHeaderComponent::textColourId, Colours::lightcyan);
}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    double rowH = getHeight() / 15;
    lbl.setBounds(0, 0, getWidth(), rowH);
    addButton.setBounds(getWidth() / 2, 0, getWidth() / 2, rowH);
    tableComponent.setBounds(0, rowH, getWidth(), rowH * 7);
    
}

int PlaylistComponent::getNumRows()
{
    return trackTitles.size();
}

void PlaylistComponent::paintRowBackground(Graphics& g,
    int rowNumber,
    int width,
    int height,
    bool rowIsSelected)
{
    if (rowIsSelected)
    {
        g.fillAll(Colours::mediumaquamarine);
    }
    else {
        g.fillAll(Colour(0xFF0b3839));
    }
}
void PlaylistComponent::paintCell(Graphics& g,
    int rowNumber,
    int columnId,
    int width,
    int height,
    bool rowIsSelected)
{
 
    if (rowNumber < getNumRows())
    {
        g.setColour(Colours::white);

        if (columnId == 1)
        {
            g.drawText(std::to_string(trackNumber[rowNumber + 1]),
                1, 0,
                width, height,
                Justification::centredLeft,
                true);
        }
        if (columnId == 2)
        {
            g.drawText(trackTitles[rowNumber],
                1, 0,
                width, height,
                Justification::centredLeft,
                true);
        }
        if (columnId == 3)
        {
            g.drawText(duration[rowNumber],
                1, 0,
                width, height,
                Justification::centredLeft,
                true);
        }
    }
  
}

Component* PlaylistComponent::refreshComponentForCell(int rowNumber,
    int columnId,
    bool isRowSelected,
    Component* existingComponentToUpdate)
{
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(Button* button)
{
        FileChooser chooser{ "" };
        if (chooser.browseForMultipleFilesToOpen())
        {
            Array<File> ids = chooser.getResults();
            for (File id : ids)
            {
                savePlaylist(id.getFullPathName().toStdString());
                insertTracks(id);
            }
        }
    tableComponent.updateContent();

}

std::string PlaylistComponent::secondsToMins(double seconds)
{
   int secs = int(seconds) % 60;
    int mins = ( seconds - (secs)) / 60;
    std::string songLength = std::to_string(mins) + ":" + std::to_string(secs);
    return songLength;
}

void PlaylistComponent::cellClicked(int rowNumber,
    int columnId,
    const MouseEvent&)
{
    selectedRowNo = trackNumber[tableComponent.getSelectedRow() + 1];
    tableComponent.repaint();
}

void PlaylistComponent::selectedRowsChanged(int lastRowSelected)
{
    selectedRowNo = trackNumber[tableComponent.getSelectedRow() + 1];
    tableComponent.repaint();
}

void PlaylistComponent::savePlaylist(std::string playlistTracks)
{
    std::ofstream outlog("playlist.txt", std::ofstream::app);
    outlog << playlistTracks << std::endl;
    outlog.close();
}

void PlaylistComponent::insertTracks(File id)
{
    auto* reader = formatManager.createReaderFor(URL{ id }.createInputStream(false));
    if (reader != nullptr) // good file!
    {
        std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader, true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
    }

    std::string title = id.getFileNameWithoutExtension().toStdString();
    std::string artist = id.getFileNameWithoutExtension().toStdString();
    std::string timeLength = secondsToMins(transportSource.getLengthInSeconds());

    trackNumber.push_back(trackNo);
    trackTitles.push_back(title);
    duration.push_back(timeLength);
    songURL.push_back(id);
    trackNo = trackNo + 1;

}


