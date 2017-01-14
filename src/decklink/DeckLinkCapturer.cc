#include <iostream>
#include <list>
#include <string>

#include "DeckLinkCapturer.hh"

/* -----------------------------------------------------------------------------
   DeckLinkCapturer
 -----------------------------------------------------------------------------*/
DeckLinkCapturer::DeckLinkCapturer(){}
DeckLinkCapturer::~DeckLinkCapturer(){}

std::list<std::string> DeckLinkCapturer::capture_video(const std::string video_filename){
    std::cout << video_filename;

    std::list<std::string> s;
    return std::move(s);    
}
