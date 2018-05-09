#ifndef __ID
#define __ID

#define MODEL_VERSION_DATA


class ID {
private:
	unsigned long long hash;
public:
#ifdef MODEL_VERSION_DATA
	int index;
#endif

	ID(unsigned long long);
	ID();
	void operator=(const ID&);
	bool operator== (const ID&) const;
	bool operator< (const ID&) const;
	bool operator> (const ID&) const;
	//int operator %(int) const;
	unsigned long long GetHash() const;
};

//ID::ID(unsigned long long h) {
//	hash = h;
//}
//bool ID::operator== (const ID& item) const {
//	return hash == item.hash;
//}
//bool ID::operator< (const ID& item) const {
//	return hash < item.hash;
//}
//bool ID::operator> (const ID& item) const {
//	return hash > item.hash;
//}
//int ID::operator %(int item) const {
//	return this->hash % item;
//}
//unsigned long long ID::GetHash() const {
//	return hash;
//}
//ID::ID() {
//	hash = 0;
//}

#endif