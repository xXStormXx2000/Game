#include "somData.h"

Component* somData::read_file(std::ifstream& file, std::string& str) {
	if (str == "somData") {
		somData* data = new somData();
		file >> data->data;
		return dynamic_cast<Component*>(data);
	}
    return nullptr;
}

void somData::write_file(std::ofstream& file) {
	file << "somData" << std::endl;
	file << this->data << std::endl;
}