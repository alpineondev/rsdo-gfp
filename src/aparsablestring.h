// razred za shranjevanje in obdelavo besedila

#ifndef AParsableStringH
#define AParsableStringH

#include "tstring.h"

class AParsableString {
  public:
    Tstring Text(void) { return T; }
    void SetText(Tstring s) { SetTextInt(s); }
    Tstring Delimiter(void) { return D; }
    void SetDelimiter(Tstring d) { D=d; }
    int DelimCount(void) { return GetDCount(); }
    int CurBytePos(void) { return GetCurBytePos(); }

    Tstring GetNextSubString(void);
    Tstring GetSubString(int Position);
    void ElimDubleDelim(void);

  private:
    // funkcije
    void SetTextInt(Tstring t);
    int GetDCount(void);
    int PosOffset(int Offset, Tstring &Text, Tstring &SearchFor);
    int GetCurBytePos(void);

    // spremenljivke
    Tstring T;  // hrani Text
    Tstring D;  // delimiter
    int x;  // pozicija (byte) zacetnega delimiterja CurPos-tega podstringa
    int x1; // pozicija (byte) koncnega delimiterja CurPos-tega podstringa
    int DCount; // stevilo delimiterjev v stringu
};
//---------------------------------------------------------------------------
#endif
