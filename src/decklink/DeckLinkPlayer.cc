#include <iostream>
#include <list>
#include <string>

#include "DeckLinkPlayer.hh"

/* -----------------------------------------------------------------------------
   DeckLinkPlayer
 -----------------------------------------------------------------------------*/
DeckLinkPlayer::DeckLinkPlayer(){}
DeckLinkPlayer::~DeckLinkPlayer(){}

void DeckLinkPlayer::play_black(){}
std::list<std::string> DeckLinkPlayer::play_video(const std::string video_filename){
    std::cout << video_filename;
    
    std::list<std::string> s;
    return std::move(s);
}
