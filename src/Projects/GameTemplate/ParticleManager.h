#ifndef PARTICLE_MANAGER_H
#define PARTICLE_MANAGER_H

#include <iostream>

#include "GameEngine/Modules/IModule.h"
#include "Particle.h"

void test(float) {}

class ParticleManager : public IModule<ParticleManager>
{
public:
   static ParticleManager* GetInstance();
   static const unsigned int kMAX_NUMBER_OF_PARTICLES;

   ParticleManager(unsigned int aNumParticle);

   virtual void GetTasks(TaskGraph& arTaskGraphContainer);
   
//private:
   static ParticleManager* Instance;

   unsigned int mNumParticles;
   Particle* mParticles;

   void Update(float fClock);
   void Update2(float fClock);
};

const unsigned int ParticleManager::kMAX_NUMBER_OF_PARTICLES = 5 * 1024;

ParticleManager* ParticleManager::Instance = 0;

ParticleManager::ParticleManager(unsigned int aNumOfParticles)
   : mNumParticles(aNumOfParticles)
{
   mParticles = new Particle[mNumParticles];

   for(unsigned int i = 0; i != mNumParticles; ++i)
   {
      mParticles[i].mPosition = Vector3(0.0f, 10.0f, 0.0f);
      mParticles[i].mVelocity = Vector3(0.0f, 0.0f, 0.0f);
   }
}

ParticleManager* ParticleManager::GetInstance()
{
   if(Instance == 0)
   {
      Instance = new ParticleManager(kMAX_NUMBER_OF_PARTICLES);
   }

   return Instance;
}

void ParticleManager::Update(float fClock)
{
   unsigned int iParticle;
   const Vector3 dampVelocity(0.75f,-0.5f,0.75f);
   const Vector3 v3Acceleration=Vector3(0.0f, -9.8f, 0.0f) * fClock;    //  a*dt
 
   for (iParticle=0; iParticle<mNumParticles; iParticle++)
   {
      Particle &p=mParticles[iParticle];
      // v' = v + a*dt
      p.mVelocity += v3Acceleration;
 
      // Update the particle position
      // p'=p + v * t + 0.5 * a * dt^2
      p.mPosition += (p.mVelocity + v3Acceleration * 0.5f) * fClock;
 
      // if Particle is below the ground plane, then damp and reverse the velocity
      if (p.mPosition.mY<0)
      {
         p.mPosition.mY*= -1;  // reflect position along y
         p.mVelocity= p.mVelocity*dampVelocity;
      }
   }

   std::cout << "done" << std::endl;
}

void ParticleManager::Update2(float t)
{
   // Not doing anything :D
   std::cout << "Particle Manager time waster" << std::endl;
}

void ParticleManager::GetTasks(TaskGraph& arTaskGraphContainer)
{
   std::vector<TaskFunctionNode> parent;
   for(int i = 0; i != 500; ++i)
   {
      TaskFunctionNode parentTask(arTaskGraphContainer.Add(CreateTask(&ParticleManager::Update2)));
      parent.push_back(parentTask);
   }
   
   arTaskGraphContainer.Add(CreateTask(&ParticleManager::Update), parent);
}

#endif