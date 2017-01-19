#include <iostream>
#include <list>
#include <string>

#include "DeckLinkMixin.hh"

class DeckLinkDriverException : public std::exception {
	virtual const char* what() const { return "This application requires the DeckLink drivers installed.\n"; }
};

class DeckLinkDeviceException : public std::exception {
	virtual const char* what() const { return "Unable to get DeckLink device.\n"; }
};

DeckLinkMixin::DeckLinkMixin(BMDDisplayMode displayMode, 
							 BMDPixelFormat pixelFormat,
							 std::ostream &logfile) : 
	m_displayMode(displayMode),
	m_pixelFormat(pixelFormat),
	m_logfile(logfile)
	 
{
	m_deckLinkIterator = CreateDeckLinkIteratorInstance();
	if (!deckLinkIterator)
	    throw DeckLinkDriverException();

	HRESULT result = m_deckLinkIterator->Next(&m_deckLink);
	if (result != S_OK || m_deckLink == NULL)
		throw DeckLinkDeviceException;
}