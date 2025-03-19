#pragma once
#include <functional>

class Entity {
	int id;
public:
	Entity(int id = -1);

	int getId() const;

    bool operator==(const Entity& other) const;

};

namespace std {
    template<>
    struct hash<Entity> {
        typedef Entity argument_type;
        typedef std::size_t result_type;

        result_type operator()(argument_type const& obj) const {
			return obj.getId();
        }
    };
}