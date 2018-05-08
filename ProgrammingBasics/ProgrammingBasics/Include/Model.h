#ifndef __MODEL
#define __MODEL

#include "Data.h"
//#define MODEL_VERSION_LINK
#define MODEL_VERSION_LINK

// ID
// IDGenerator
// Library //
// INumerable
// Queue
// Dequeue
// List
// Array
// Dictionary
// Pair-Tree
// Set
// Hash-Table
// Type
// Vector2
// Primitives
// Requirement
// Logger
// Data
// Model

typedef BinSearchTree<ID, List<ID>*> DataLink;
typedef BinSearchTree<ID, Primitive*> DataPrim;
typedef BinSearchTree<ID, Requirement*> DataReq;

class Model
{
private:

	DataPrim dataPrim;

	DataReq dataReq;

	DataLink dataLink;

	IDGenerator* idGen;

	// Evgeny' graph
	Data data; 

#ifdef MODEL_VERSION_LINK 
	void GetIDRequirementsInComponent(const ID&, Array<ID>&);


	void FindRequirementsByID(Array<ID>&, Array<Requirement*>&);

	//may be not using
	bool find(const ID&, Array<ID>);

	bool find(const ID&, Array<Primitive*>&);

	bool find(const ID&, Array<Requirement*>&);

	void ConnectPrimitives(Primitive*, Primitive*);

	bool CreateRequirement(req_type, Array<Primitive*>&, Array<double>&);
#endif

	//similar functions!!
	double GetError();
	double GetError(const Array<Requirement*>&) const;
	//

	double ErrorByAlpha(const Array<Requirement*>&, const Array<double*>&, const Array<double>&, double);
	void OptimizeByGradient(const Array<Requirement*>&, const Array<double*>&, const Array<double>&);


public:
	class infoObject
	{
	public:
		infoObject(){ }
		void operator=(const infoObject& input)
		{
			this->params = input.params;
			this->type = input.type;
		}
		Array<double> params;
		prim_type type;
	};

	Model() { }

	// Create destructor
	~Model() { }

	bool DischargeInfoObjects(Array<infoObject>&);

#ifdef MODEL_VERSION_LINK
	bool CreateObject(prim_type, Array<double>&, ID&);
	
	bool CreateRequirementByID(req_type, Array<ID>&, Array<double>&);
	void CreateLink(const ID&, Array<Primitive*>&);

	bool DeletePrimitive(const ID&);
	bool DeleteRequirement(const ID&);

	bool GetComponent(const ID&, BinSearchTree<ID, ID>&);
#endif

	bool getNearest(double, double, ID&, double&);

	bool getObjType(const ID&, prim_type&);

	////replace with GET CHILD 
	//bool GetSegmentPoints(ID, Array<ID>&);
	//bool GetArcPoints(ID, Array<ID>&);

	bool getObjParam(const ID&, Array<double>&);
	
	void OptimizeRequirements(const Array<Requirement*>&);
	// void PrintSystemRequirement();

	//temp function
	void OptimizeAllRequirements();

	// XXX Function

#ifdef MODEL_VERSION_DATA
	bool XXXCreateObject(const prim_type, Array<double>&, ID&);

	void XXXConnectPrimitives(Primitive*, Primitive*, Primitive*);

	bool XXXCreateRequirementByID(const req_type, Array<int>&, Array<ID>&, Array<double>&);

	bool XXXCreateRequirement(const req_type, Array<int>&, Array<ID>&, Array<Primitive*>&, Array<double>&);

	void XXXGetRequirementsByID(int, const ID&, Array<Requirement*>&);

	void XXXGetRequirementsIDByID(int, const ID&, Array<ID>&);

	void XXXGetRequirementsType(int, const ID&, Array<req_type>&);

	void XXXDeleteRequirement(int, const ID&);

	void XXXDeletePrimitive(int, const ID&);
#endif
};
#endif