#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "ray.hpp"
#include "color.hpp"
#include "material.hpp"
#include "hit.hpp"

#include <string>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtx/intersect.hpp>
#include <vector>

class Shape
{
public:
	Shape();
	Shape(std::string const& name, Material const& mat);
	Shape(std::string const& name, Material const& mat, glm::mat4 const& transe);
	~Shape();
	std::string const& getname() const;
	Material const& getmat() const;

	void  setname(std::string const& s);
	void  setmat(Material  const&    m);

	virtual Hit intersect(Ray const & ray) = 0;

	virtual std::ostream & print ( std :: ostream & os ) const ;

	Ray transformRay(glm::mat4 const& mat, Ray const& ray);

<<<<<<< HEAD
	Ray translate(glm::mat4 const& trans, Ray const& ray);
	Ray scale(glm::mat4 const& sca, Ray const& ray);
=======
	inline void makeMatrix(glm::mat4 const& mat)
	{
		//world_transformation * = mat;
	}

>>>>>>> 03a7a03dc20cb7768e706afd5b65802e462f4b22
	//Composite
	virtual void removeChild(std::shared_ptr<Shape>& shape){}
	virtual void addChild(std::shared_ptr<Shape>& shape){}
	virtual std::vector<std::shared_ptr<Shape>> getChild(){}
	

protected:
	std::string name_;
	Material mat_;
	glm::mat4 world_transformation;
	glm::mat4 world_transformation_inv;
};

std::ostream& operator<<(std::ostream& os, Shape const& s);

#endif
