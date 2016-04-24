//////////////////////////////////////////////////////
//
//  stlext.h
//
//  Some of my extensions to STL.
//  This file included in stdAfx.h, so no need to include elsewhere
//
///////////////////////////////////////////////////////

#ifndef _stlext_included_
#define _stlext_included_



//  This file contains templated functions that provide some
//  useful functionality for playing with STL.



//  Stole this from ObjectSpace
//  We often have vectors and sets of pointers.  
//	Sets require a comparitor function.
//  Vectors we often need to sort.
//  In both cases, we don't usually want to compare the pointers,
//    we want to compare the objects being pointed to.
//  There is a standard usage involving less:
//
//     set<int,less<int> > intset;
//	   vector<int> v;
//	   sort(v.begin(),v.end(),less<int>());
//
//  Usage of less_p is analogous for sets and vectors of pointers:
//
//	   set<T*, less_p<T*> > Tset;
//	   vector<T*> v;
//     sort(v.begin(),v.end(),less_p<T*>());
//

template<class T> class less_p : public binary_function<T,T,bool>
{
	
public:
	less_p() {};

	bool operator() (const T& x_, const T& y_) const
	{
		return (*x_) < (*y_);
	}
};




//  This _used_ to be in the STL standard, under the name release
//	This implementation also stolen from ObjectSpace
//  
//
//  Purpose: deletes all pointers in a collection delineated by
//   two iterators.
//
//  Example:  
//
//		vector<T*> v;
//      ...
//      release(v.begin(),v.end());


template<class FwdIt> void release(FwdIt first_, FwdIt last_)
{
	for( ; first_ != last_; ++first_ )
		delete *first_;
}
		


//  Similar to release, but uses delete[] instead of delete.
//  Example:  
//
//		vector<char*> v;
//		...
//		v.push_back(strdup("some string"));
//		...
//		release_arrays(v.begin(),v.end());
//
//  I can't think of very many uses for this, other than the
//  example of char arrays created with new[].


template<class FwdIt> void release_arrays(FwdIt first_, FwdIt last_)
{
	for( ; first_ != last_; ++first_ )
		delete [] *first_;
}


#endif



