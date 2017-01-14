#pragma once

#include <string>
#include <list>
#include "DeckLinkMixin.hh"

class DeckLinkPlayer : public DeckLinkMixin {
public:
    DeckLinkPlayer();
    ~DeckLinkPlayer();

    void play_black();
    std::list<std::string> play_video(const std::string video_filename);
};
