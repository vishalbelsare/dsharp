// Class automatically generated by Dev-C++ New Class wizard

#ifndef ATOMSANDNODES_H
#define ATOMSANDNODES_H

#include <vector>

#include <iostream>

#include "../../Basics.h"
#include <SomeTime.h>

using namespace std;

#define INVALID_DL -1

class CClauseVertex;
typedef CClauseVertex *PCLV;

class CVariableVertex;
typedef CVariableVertex *PVARV;

typedef unsigned int indexTypeVARS;
typedef unsigned int indexTypeCLS;

typedef indexTypeVARS VarIdT;
typedef indexTypeCLS ClauseIdT;

class LiteralIdT
{
	indexTypeVARS val;
public:

	LiteralIdT()
	{
		val = 0;
	}
	LiteralIdT(int iLitIdx)
	{
		val = (abs(iLitIdx) << 1) + (unsigned int) (iLitIdx > 0);
	}

	LiteralIdT(VarIdT iVarIdx, bool polarity)
	{
		val = (iVarIdx << 1) + (unsigned int) polarity;
	}

	VarIdT toVarIdx() const
	{
		return (val >> 1);
	}

	unsigned int toUInt() const
	{
		return val;
	}

	void makeFromUInt(unsigned int v)
	{
		val = v;
	}

	bool polarity() const
	{
		return (bool) (val & 0x01);
	}

	int toSignedInt() const
	{
		return polarity() ? toVarIdx() : (-1 * toVarIdx());
	}

	TriValue polarityTriVal() const
	{
		return (TriValue) (val & 0x01);
	}

	bool operator!=(const LiteralIdT &rL2) const
	{
		return val != rL2.val;
	}

	bool operator==(const LiteralIdT &rL2) const
	{
		return val == rL2.val;
	}

	const LiteralIdT oppositeLit() const
	{
		return LiteralIdT(toVarIdx(), !polarity());
	}

	void print() const
	{
		toSTDOUT((polarity()?" ":"-")<< toVarIdx()<<" ");
	}
};

static const ClauseIdT NOT_A_CLAUSE(0);
static const LiteralIdT NOT_A_LIT(0, false);

#define SENTINEL_CL NOT_A_CLAUSE
#define SENTINEL_LIT NOT_A_LIT

class CClauseVertex
{
	/// the offset of the first literal of this clause in theLitVector of
	/// the main CInstanceGraph
	unsigned int litPoolOfs;

	LiteralIdT litA, litB;

	unsigned int numLits;
	int lastTouchTime;

	bool conflictClause;

public:

	bool isCC()
	{
		return conflictClause;
	}

	bool hasWatchLitA()
	{
		return litA != NOT_A_LIT;
	}
	bool hasWatchLitB()
	{
		return litB != NOT_A_LIT;
	}

	void setLitA(const LiteralIdT & lit)
	{
		litA = lit;
	}
	void setLitB(const LiteralIdT & lit)
	{
		litB = lit;
	}

	const LiteralIdT &idLitA() const
	{
		return litA;
	}
	const LiteralIdT &idLitB() const
	{
		return litB;
	}

	int getLastTouchTime() const
	{
		return lastTouchTime;
	}

	void setTouched()
	{
		lastTouchTime = CStepTime::getTime();
	}

	void setLength(unsigned int nlen)
	{
		numLits = nlen;
	}
	unsigned int length() const
	{
		return numLits;
	}

	bool isDeleted() const
	{
		return numLits == 0;
	}

	void setDeleted()
	{
		numLits = 0;
	}

	void setLitOfs(unsigned int ofs)
	{
		litPoolOfs = ofs;
	}
	unsigned int getLitOfs() const
	{
		return litPoolOfs;
	}

	// class constructor
	CClauseVertex(bool isConflict)
	{
		litA = litB = NOT_A_LIT;
		numLits = 0;
		litPoolOfs = 0;
		lastTouchTime = 0;
		conflictClause = isConflict;
	}

	void reset()
	{

		lastTouchTime = 0;
	}

	~CClauseVertex()
	{
	}

	bool watches(const VarIdT &theVar) const
	{
		return (idLitA().toVarIdx() == theVar || idLitB().toVarIdx() == theVar);
	}

};

template<typename _T>
class extd_vector: public vector<_T>
{

public:
	void quickErase(int iOfs)
	{
		(*this)[iOfs] = vector<_T>::back();
		vector<_T>::pop_back();
	}

	//typename vector<_T>::iterator
	void quickErase(typename vector<_T>::iterator &it)
	{
		*it = vector<_T>::back();
		vector<_T>::pop_back();
	}

};

class AntecedentT
{
	unsigned int val;

public:
	AntecedentT()
	{
		val = 1;
	}
	AntecedentT(const ClauseIdT idCl)
	{
		val = (idCl << 1) | 1;
	}
	AntecedentT(const LiteralIdT idLit)
	{
		val = (idLit.toUInt() << 1);
	}

	bool isAClause()
	{
		return val & 0x01;
	}

	ClauseIdT toCl()
	{
		return val >> 1;
	}
	LiteralIdT toLit()
	{
		LiteralIdT idLit;
		idLit.makeFromUInt(val >> 1);
		return idLit;
	}
	bool isAnt()
	{
		return (isAClause() && !(toCl() == NOT_A_CLAUSE)) || (!isAClause()
				&& (toLit() != NOT_A_LIT));
	}

};

class AntAndLit
//structure saving a Lit and the antecedent implying it
// antecedents may be clauses or literals
{
public:
	AntecedentT theAnt;
	LiteralIdT theLit;

	AntAndLit(const AntecedentT ant, const LiteralIdT pL)
	{
		theAnt = ant;
		theLit = pL;
	}

	AntAndLit(const LiteralIdT idAntLit, const LiteralIdT pL)
	{
		theAnt = AntecedentT(idAntLit);
		theLit = pL;
	}

	AntAndLit(const ClauseIdT idCl = NOT_A_CLAUSE, const LiteralIdT pL = 0)
	{
		theAnt = AntecedentT(idCl);
		theLit = pL;
	}

	AntecedentT &getAnt()
	{
		return theAnt;
	}
	LiteralIdT& getLit()
	{
		return theLit;
	}
};

class CVariableVertex
{
	/// INVARIANT : SENTINEL clclcl ...
	/// this is due to the watchCls being passed in reverse by BCP
	extd_vector<ClauseIdT> watchCls[2];

	TriValue theVal;

	VarIdT itecIndex;

	AntecedentT myAntecedent;
	int DecLevelOfDeactivation;

	/// INVARIANT: array content: llll SENTINEL xxxxx SENTINEL
	/// x: conflict binary link
	vector<LiteralIdT> binClLinks[2]; // where the variable occurrs in pos/neg

	unsigned int inClsVecOfs[2]; // the offset in the theInClsVector
	unsigned int iVarNum; // the number the variable had in the input file


public:

	void setInClsVecOfs(bool pol, unsigned int ofs)
	{
		inClsVecOfs[pol] = ofs;
	}

	void setInClsVecOfs(unsigned int ofs)
	{
		unsigned int diff = inClsVecOfs[true] - inClsVecOfs[false];
		inClsVecOfs[false] = ofs;
		inClsVecOfs[true] = ofs + diff;
	}

	unsigned int getInClsVecOfs(bool pol) const
	{
		return inClsVecOfs[pol];
	}

	int scoreDLIS[2];
	int scoreVSIDS[2];

	void eraseAllEdges()
	{
		inClsVecOfs[0] = 1;
		inClsVecOfs[1] = 1;

		watchCls[0].clear();
		watchCls[0].push_back(SENTINEL_CL);
		watchCls[1].clear();
		watchCls[1].push_back(SENTINEL_CL);

		binClLinks[0].clear();
		binClLinks[0].push_back(SENTINEL_LIT);
		binClLinks[0].push_back(SENTINEL_LIT);
		binClLinks[1].clear();
		binClLinks[1].push_back(SENTINEL_LIT);
		binClLinks[1].push_back(SENTINEL_LIT);
	}

	CVariableVertex(unsigned int numofVar = 0, VarIdT index = 0) :
		itecIndex(index), iVarNum(numofVar)
	{
		DecLevelOfDeactivation = INVALID_DL;
		theVal = X;

		scoreDLIS[0] = scoreDLIS[1] = 0;
		scoreVSIDS[0] = scoreVSIDS[1] = 0;

		eraseAllEdges();
	}

	bool isolated()
	{
		return (inClsVecOfs[0] <= 1) && (inClsVecOfs[1] <= 1)
				&& (binClLinks[0].size() <= 2) && (binClLinks[1].size() <= 2);
	}

	LiteralIdT getLitIdT(bool polarity)
	{
		return LiteralIdT(getVarIdT(), polarity);
	}

	void newtecIndex(VarIdT iIdx)
	{
		itecIndex = iIdx;
	}

	int getDLCSScore() const
	{
		return scoreDLIS[0] + scoreDLIS[1];
	}

	int getVSIDSScore()
	{
		return scoreVSIDS[0] + scoreVSIDS[1];
	}

	inline const unsigned int getVarNum() const
	{
		return iVarNum;
	}

	inline const unsigned int getVarIdT() const
	{
		return itecIndex;
	}

	int getDLOD() const
	{
		return DecLevelOfDeactivation;
	}

	bool setVal(bool aVal, unsigned int atDL, AntecedentT ant = AntecedentT(
			NOT_A_CLAUSE))
	{
		if (!isActive())
			return false;
		DecLevelOfDeactivation = atDL;

		myAntecedent = ant;
		theVal = (TriValue) aVal;
		return true;
	}

	void unsetVal()
	{
		myAntecedent = AntecedentT(NOT_A_CLAUSE);
		DecLevelOfDeactivation = INVALID_DL;
		theVal = X;
	}

	bool isImpliedBy(const ClauseIdT idCl)
	{
		if (isActive() || !myAntecedent.isAClause())
			return false;
		if (myAntecedent.toCl() == idCl)
			return true;
		return false;
	}
	void adjustAntecedent(AntecedentT ant)
	{
		myAntecedent = ant;
	}
	bool isActive() const
	{
		return theVal == X;
	}

	bool flipVal()
	{
		if (theVal == W)
			theVal = F;
		else if (theVal == F)
			theVal = W;

		return true;
	}

	AntecedentT getAntecedent()
	{
		return myAntecedent;
	}

	bool getboolVal() const
	{
		return theVal == W;
	}

	TriValue getVal() const
	{
		return theVal;
	}

	unsigned int countBinLinks() const
	{
		return binClLinks[0].size() + binClLinks[1].size() - 4;
	}
	unsigned int countBinLinks(bool pol) const
	{
		return binClLinks[pol].size() - 2;
	}

	extd_vector<ClauseIdT> &getWatchClauses(bool polarity)
	{
		return watchCls[polarity];
	}

	const extd_vector<ClauseIdT> &getWatchClauses(bool polarity) const
	{
		return watchCls[polarity];
	}

	void addWatchClause(ClauseIdT idCl, bool polarity)
	{
		watchCls[polarity].push_back(idCl);
	}

	void addBinLink(bool polarity, LiteralIdT &lit)
	{
		*(binClLinks[polarity].end() - 2) = lit;
		binClLinks[polarity].push_back(SENTINEL_LIT);
	}

	void addBinLinkCCl(bool polarity, LiteralIdT &lit)
	{
		*(binClLinks[polarity].end() - 1) = lit;
		binClLinks[polarity].push_back(SENTINEL_LIT);
	}

	bool eraseBinLinkTo(LiteralIdT idLit, bool Linkpolarity);
	bool hasBinLinkTo(LiteralIdT idLit, bool pol) const;

	const vector<LiteralIdT> & getBinLinks(bool polarity) const
	{
		return binClLinks[polarity];
	}
	vector<LiteralIdT> & getBinLinks(bool polarity)
	{
		return binClLinks[polarity];
	}

	bool eraseWatchClause(ClauseIdT idCl, bool polarity);

	bool substituteWatchCl(bool polarity, const ClauseIdT & oldId,
			const ClauseIdT &newId);

	bool substituteBinLink(bool polarity, const LiteralIdT& oldLit,
			const LiteralIdT& newLit);

	// class destructor
	~CVariableVertex();
};

#endif
