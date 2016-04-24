// OrderColor.cpp: implementation of the COrderColor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "csda.h"
#include "OrderColor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#include "CSOrder.h"
#include "Order.h"


//////////////////////////////////////////////////////////////////////
//	COrderColor
//
//  Provides standard colors based on first two digits of condition code.
//	Used in the gantt chart.
//
//////////////////////////////////////////////////////////////////////



COrderColor::COrderColor()
{

}

COrderColor::~COrderColor()
{

}


#define C0 0x58
#define C1 0x60
#define C2 0x68
#define C3 0x80
#define C4 0x98
#define C5 0xB0
#define C6 0xC8
#define C7 0xE0
#define C8 0xE8
#define C9 0xF0


#define G0 0x20
#define G1 0x24
#define G2 0x28
#define G3 0x38
#define G4 0x48
#define G5 0x58
#define G6 0x60
#define G7 0x68
#define G8 0x70
#define G9 0x78

#define B0 RGB(C0,G0,G0)
#define B1 RGB(C1,G1,G1)
#define B2 RGB(C2,G2,G2)
#define B3 RGB(C3,G3,G3)
#define B4 RGB(C4,G4,G4)
#define B5 RGB(C5,G5,G5)
#define B6 RGB(C6,G6,G6)
#define B7 RGB(C7,G7,G7)
#define B8 RGB(C8,G8,G8)
#define B9 RGB(C9,G9,G9)

#define BadRow { B0, B1, B2, B3, B4, B5, B6, B7, B8, B9 }




COLORREF COrderColor::colors[10][10]
= { 
	BadRow, // 1stDigit = 0 -- no processing

	// 1stDigit = 1 -- to dock
	{ RGB(C0,C0,0),
	  RGB(C1,C1,0),
	  RGB(C2,C2,0),
	  RGB(C3,C3,0),
	  RGB(C4,C4,0),
	  RGB(C5,C5,0),
	  RGB(C6,C6,0),
	  RGB(C7,C7,0),
	  RGB(C8,C8,0),
	  RGB(C9,C9,0) },

	BadRow, // 1stDigit = 2
	BadRow, // 1stDigit = 3

	// 1stDigit = 4 -- around dock
	{ RGB(G0,C0,G0),
	  RGB(G1,C1,G1),
	  RGB(G2,C2,G2),
	  RGB(G3,C3,G3),
	  RGB(G4,C4,G4),
	  RGB(G5,C5,G5),
	  RGB(G6,C6,G6),
	  RGB(G7,C7,G7),
	  RGB(G8,C8,G8),
	  RGB(G9,C9,G9) },

	// 1stDigit = 5 -- CMS
	{ RGB(0,C0,C0),
	  RGB(0,C1,C1),
	  RGB(0,C2,C2),
	  RGB(0,C3,C3),
	  RGB(0,C4,C4),
	  RGB(0,C5,C5),
	  RGB(0,C6,C6),
	  RGB(0,C7,C7),
	  RGB(0,C8,C8),
	  RGB(0,C9,C9) },

	BadRow, // 1stDigit = 6
	BadRow, // 1stDigit = 7

	// 1stDigit = 8

	{ RGB(G0,G0,C0),
	  RGB(G1,G1,C1),
	  RGB(G2,G2,C2),
	  RGB(G3,G3,C3),
	  RGB(G4,G4,C4),
	  RGB(G5,G5,C5),
	  RGB(G6,G6,C6),
	  RGB(G7,G7,C7),
	  RGB(G8,G8,C8),
	  RGB(G9,G9,C9) },

	// 1stDigit = 9

	{ RGB(C0,0,C0),
	  RGB(C1,0,C1),
	  RGB(C2,0,C2),
	  RGB(C3,0,C3),
	  RGB(C4,0,C4),
	  RGB(C5,0,C5),
	  RGB(C6,0,C6),
	  RGB(C7,0,C7),
	  RGB(C8,0,C8),
	  RGB(C9,0,C9) }

};



COLORREF COrderColor::GetColor(CCSOrder* pOrder)
{
	return GetColor(pOrder->SlabCondnCode());
}

COLORREF COrderColor::GetColor(COrder* pOrder)
{
	return GetColor(pOrder->SlabCondnCode());
}


COLORREF COrderColor::GetColor(int condnCode)
{
	// the condition code is 4-digit.

	int dig1 = (condnCode / 1000) % 10;
	int dig2 = (condnCode / 100 ) % 10;

	return colors[dig1][dig2];
}
