#include "renderer.hpp"
#include "vektoroperations.hpp"
#include <iostream>
#include "camera.hpp"
#include <math.h>

Renderer::Renderer(Scene const& scene, unsigned w, unsigned h, std::string const& file):
  scene_(scene),
  width_(w),
  height_(h), 
  colorbuffer_(w*h, Color(0.0, 0.0, 0.0)), 
  filename_(file), 
  ppm_(width_, height_)
{}

void Renderer::render()
{
  if(scene_.sizeShape > 0)
  {
    for (unsigned y = 0; y < height_; ++y) 
    {
      for (unsigned x = 0; x < width_; ++x) 
      {
        Pixel p(x,y);
        
        Ray ronny = scene_.cam->calculateRay(x,y);
        
        p.color = raytrace(ronny,x);

        write(p);
      }
    }
  }
  else
  {
    std::cout<<"Es wurden keine Objekte geladen."<<std::endl;
  }
  ppm_.save(filename_);
}

Color Renderer::raytrace(Ray const& ronny,unsigned int depth) const
{
  //depth --;

  Hit hit = calculateHit(ronny);

  if (hit.impact)
      {
        Color c{0,0,0};
        if(true)//illuminate(hit.point))
        {
          Material mat{hit.shape->getmat()};

          float iamb = 0.15;

          float ambientR = iamb * mat.ka_.r;
          float ambientG = iamb * mat.ka_.g;
          float ambientB = iamb * mat.ka_.b;

          if(ambientR < 0)ambientR=0;
          if(ambientG < 0)ambientG=0;
          if(ambientB < 0)ambientB=0;         

          float diffuseR = scene_.lights[0]->intensity * mat.kd_.r * (skalar(glm::normalize(hit.normal),glm::normalize(scene_.lights[0]->pos - hit.point)) );
          float diffuseG = scene_.lights[0]->intensity * mat.kd_.g * (skalar(glm::normalize(hit.normal),glm::normalize(scene_.lights[0]->pos - hit.point)) );
          float diffuseB = scene_.lights[0]->intensity * mat.kd_.b * (skalar(glm::normalize(hit.normal),glm::normalize(scene_.lights[0]->pos - hit.point)) );

          if(diffuseR < 0)diffuseR=0;
          if(diffuseG < 0)diffuseG=0;
          if(diffuseB < 0)diffuseB=0;

          glm::vec3 ausfallvektor = mirror(scene_.lights[0]->pos , Ray{hit.point, hit.normal});


          float specularR = scene_.lights[0]->intensity * mat.ks_.r * (mat.m_ + 2) / 6.2831 * pow(skalar(ausfallvektor ,glm::normalize(ronny.direction)), mat.m_);
          float specularG = scene_.lights[0]->intensity * mat.ks_.g * (mat.m_ + 2) / 6.2831 * pow(skalar(ausfallvektor ,glm::normalize(ronny.direction)), mat.m_);
          float specularB = scene_.lights[0]->intensity * mat.ks_.b * (mat.m_ + 2) / 6.2831 * pow(skalar(ausfallvektor ,glm::normalize(ronny.direction)), mat.m_);

          if(specularR < 0)specularR=0;
          if(specularG < 0)specularG=0;
          if(specularB < 0)specularB=0;

          c.r = ambientR/3 + diffuseR/3 + specularR/3;
          c.g = ambientG/3 + diffuseG/3 + specularG/3;
          c.b = ambientB/3 + diffuseB/3 + specularB/3;

          /*c.r = iamb * mat.ka_.r + scene_.lights[0]->intensity * (mat.kd_.r * skalar (glm::normalize(scene_.lights[0]->pos - hit.point), glm::normalize(hit.normal)))
                + mat.ks_.r * ( (mat.m_ + 2) / 6.2831 ) *  pow(skalar(ausfallvektor ,glm::normalize(ronny.direction)), mat.m_);
         */

        }
        else
        {
          c.r = c.g = c.b = 0;
        }
        /*float a = 1; 
        if(!illuminate(hit.point))
          {
            a = a / 2;
          }
        glm::vec3 debugNormal = 0.5f * hit.normal + glm::vec3(0.5);
        Color c = Color(a*debugNormal.x,a*debugNormal.y,a*debugNormal.z);*/
        return c;
      } 

}
Hit Renderer::calculateHit(Ray const& rafa) const
{
  Hit hit = scene_.shapes[0]->intersect(rafa);
  
  for(unsigned int i = 1; i < scene_.sizeShape; i++)
  {
    if(scene_.shapes[i] != nullptr)
    {
      Hit newHit = scene_.shapes[i]->intersect(rafa);

        if(!hit.impact || (newHit.impact && glm::length(newHit.point - rafa.origin) < glm::length(hit.point - rafa.origin)))
        {
          hit = newHit;
        }
    }
    else std::cout<< "scene_.shapes["<<i<<"] == nullptr"<<std::endl;
  }
  return hit;
}

bool Renderer::illuminate(glm::vec3 const& point ) const
{
  for(unsigned i =0; i < scene_.sizeLight; i++)
  {
    Ray robert{ point,scene_.lights[i]->pos - point};

    Hit isilluminated = calculateHit(robert);

    if(!isilluminated.impact)
    {
      return true;
    }
    else if(glm::length( point - scene_.lights[i]->pos) < glm::length( point - isilluminated. point))
    {
      return true;
    }
    else 
    {
      return false;
    }
  }
}

void Renderer::write(Pixel const& p)
{
  // flip pixels, because of opengl glDrawPixels
  size_t buf_pos = (width_*p.y + p.x);
  if (buf_pos >= colorbuffer_.size() || (int)buf_pos < 0) {
    std::cerr << "Fatal Error Renderer::write(Pixel p) : "
      << "pixel out of ppm_ : "
      << (int)p.x << "," << (int)p.y
      << std::endl;
  } else {
    colorbuffer_[buf_pos] = p.color;
  }

  ppm_.write(p);
}