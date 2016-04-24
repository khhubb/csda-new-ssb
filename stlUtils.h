////////////////////////////////////////////////////////////////////////
//
//  stlUtils.h
//
//  Miscellaneous utility functions
//  This file included in stdAfx.h, so no need to include elsewhere
//
//
////////////////////////////////////////////////////////////////////////


#ifndef _stlUtils_included_
#define _stlUtils_included_


//  Transfers items (indicated by position) from one vector to another.

//  We have to handle two cases: toVec and fromVec are the same vector,
//     or not.
//  When they are the same vector, 
//     the act of inserting the items invalidates the positions 
//         so we can't delete them from their original positions,
//  or the act of deleting the items invalidates the insertIter,
//         so we can't insert them into their new position.
//  So, when we have the same vector, we compute how many positions 
//    lie before the insertIter so we can adjust its position.
//  Of course, if we are inserting at the end, it doesn't matter.


template<class T>
void TransferVectorItems(vector<T>& toVec,
						 typename vector<T>::iterator insertIter,
						 vector<T>& fromVec,
						 const vector<int>& positions)
{
	bool insertAtEnd = (toVec.end() == insertIter);

	int iterIndex = insertIter - toVec.begin();
	int count = 0;

	if ( &toVec == &fromVec  &&  ! insertAtEnd ) {

		// Same vector, not at end.
		// Compute the index of the iterator.
		
		for ( vector<int>::const_iterator ii = positions.begin();
			  ii != positions.end();
			  ++ii ) 

			if ( (*ii) < iterIndex )
				++count;


	}
	
	iterIndex -= count;
	
	// Let's make sure I didn't screw up.

	assert ( insertAtEnd || 0 <= iterIndex && iterIndex < toVec.size() );

	//  Xfer the selected items to a temporary vector
	
	vector<T> temp;
	
	for ( vector<int>::const_iterator ii = positions.begin();
		  ii != positions.end();
		  ++ii )
		
		temp.push_back(fromVec[*ii]);
		  
		  
	// Delete the items from their original positions
	DeletePositionsFromVector(fromVec,positions);


	// insert the items into the target vector.
	if ( insertAtEnd ) 

		toVec.insert(toVec.end(),temp.begin(),temp.end());
  
	else 

	  toVec.insert(toVec.begin()+iterIndex,temp.begin(),temp.end());
  
}


// Similar, except we don't delete.
// this simplifies things tremendously.
 
template<class T>
void CopyVectorItems(vector<T>& toVec,
					typename vector<T>::iterator insertIter,
						 vector<T>& fromVec,
						 const vector<int>& positions)
{
	//  Xfer the selected items to a temporary vector
	
	vector<T> temp;
	
	for ( vector<int>::const_iterator ii = positions.begin();
		  ii != positions.end();
		  ++ii )
		
		temp.push_back(fromVec[*ii]);

	toVec.insert(insertIter,temp.begin(),temp.end());
}




//  delete items given by index from a vector

template<class T>
void DeletePositionsFromVector(vector<T>& vec,
						       const vector<int>& positions)
{
	//  We arrange to delete items from the end of the source vector.
	//  First sort the positions to remove into decreasing order.

	vector<int> sortPosns(positions);
	sort(sortPosns.begin(),
		sortPosns.end(),
		greater<int>() );
	

	//  Now erase the indicated positions from the vector

	for ( vector<int>::const_iterator i = sortPosns.begin();
		  i != sortPosns.end();
		  ++i ) 

		vec.erase(vec.begin()+(*i));
	
}





// Given a vector of items (targetV),
//   all of which also occur in some other vector (parentV),
//   add the indexes in parentV of the items from targetV to positions.

template<class T>
void FindIndexesOfItems(vector<T>& targetV,
						vector<T>& parentV,
						vector<int>& positions)
{

	positions.reserve(targetV.size());
	positions.erase(positions.begin(),positions.end());

	for ( vector<T>::iterator iTgt = targetV.begin();
		  iTgt != targetV.end();
		  ++iTgt ) {

		vector <T>::iterator iParentV = find(parentV.begin(),
											 parentV.end(),
											 (*iTgt));

		assert (iParentV != parentV.end() );
		
		positions.push_back(iParentV - parentV.begin());
	}
}


//  want to do the following:
//
//      target.insert(source.begin(),source.end())
//
//  but compiler won't let me.  Not sure why.
//  So I do it myself.
//  Because both are ordered the same way,
//     we can get a more efficient insertion
//     by recording the place of the last insertion.

template<class T>
void InsertSet(set<T>& target,
			   set<T>& source)
{
	if ( source.empty() )
		return;

	set<T>::const_iterator is = source.begin();

	pair< set<T>::iterator, bool> p = target.insert( (*is) );
	set<T>::iterator it = p.first;

	++is;

	for (  ; is != source.end(); ++is ) 
		it = target.insert(it,(*is));
}





#endif //  _stlUtils_included_
