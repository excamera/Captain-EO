#pragma once

#include <string>
#include <list>
#include "DeckLinkMixin.hh"

class DeckLinkCapturer : public DeckLinkMixin {
public:
    DeckLinkCapturer();
    ~DeckLinkCapturer();
    
    std::list<std::string> capture_video(const std::string video_filename);

private:    
};
