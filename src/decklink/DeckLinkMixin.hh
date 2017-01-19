#pragma once

#include <string>
#include <list>
#include <iostream>
#include <fstream>

#include "DeckLinkAPI.h"

class DeckLinkMixin {

public:
	DeckLinkMixin(BMDDisplayMode displayMode, 
				  BMDPixelFormat pixelFormat,
				  std::ostream &logfile);

protected:
	IDeckLinkIterator* 		m_deckLinkIterator;
	IDeckLink*				m_deckLink;
	BMDDisplayMode			m_displayMode;

	std::ostream			&logfile;
};
