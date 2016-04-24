// SpecCmp.h: interface for the CSpecCmp class.
//
//////////////////////////////////////////////////////////////////////
 
#if !defined(AFX_SPECCMP_H__B4BA9EEB_58B7_11D1_8054_006097B38214__INCLUDED_)
#define AFX_SPECCMP_H__B4BA9EEB_58B7_11D1_8054_006097B38214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000



//  Whoooo, now here's a goodie.
//
//  We want to search a vector of specs to see
//   if there is a match to a given spec.
//  We could use find, but this does not use a straight string comparison.
//  So we introduce a class which is a functional (overloads ())
//     and which has the given spec as a parm.

class CSpecCmp : public unary_function<const CString&,bool>
{
public:

	CSpecCmp(const CString& targ) : pTarget(&targ) {}

	void SetString(const CString& targ) { pTarget = &targ; }
	const CString& GetString() { return *pTarget; }

	const CString* pTarget;

	
	// 
	//  X counts as a wild card for matching.
	//  Thus 555-21-00 will match 555-2X-XX
	//

	static inline bool charMatch(const CString& x,
						  const CString& y,
						  int i)
	{
		return x[i] == y[i] || x[i] == 'X' || y[i] == 'X';
	}

	static inline bool compare(const CString& x,
					    const CString& y)
	{
		if ( x.GetLength() < 6 || y.GetLength() < 6 )
			return ( x == y );

		return charMatch(x,y,0)
			   &&
			   charMatch(x,y,1)
			   && 
			   charMatch(x,y,2)
			   &&
			   charMatch(x,y,3)
			   &&
			   charMatch(x,y,4)
			   &&
			   charMatch(x,y,5);
	}

	bool operator()( const CString& x ) const
	{
		//return ( *pTarget == x );
		return compare(*pTarget,x);
	}

};
		


#endif // !defined(AFX_SPECCMP_H__B4BA9EEB_58B7_11D1_8054_006097B38214__INCLUDED_)
