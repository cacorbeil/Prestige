#ifdef TASK_NODE_H

namespace GameEngine
{
   typedef Util::IncrementatorType ModuleId;

   template<typename Type>
   TaskNode<Type>::TaskNode()
      : mTask()
      , mId(0)
      , mParentIdList()
   {
   }


   template<typename Type>
   TaskNode<Type>::TaskNode(const Type& aTask)
      : mTask(aTask)
      , mId(Util::Incrementator<INCREMENTATOR_ENUM::TASK_ID>::GetIncrement())
      , mParentIdList()
   {
   }

   template<typename Type>
   TaskNode<Type>::TaskNode(const Type& aTask, std::vector<Id> aChildrenIdList)
      : mTask(aTask)
      , mId(Incrementator<INCREMENTATOR_ENUM::TASK_ID>::GetIncrement())
      , mParentIdList(aChildrenIdList)
   {
   }
   template<typename Type>
   TaskNode<Type>::TaskNode(const TaskNode<Type>& arTaskNode)
      : mTask(arTaskNode.mTask)
      , mId(arTaskNode.mId)
      , mParentIdList(arTaskNode.mParentIdList)
   {
   }

   template<typename Type>
   TaskNode<Type>::~TaskNode()
   {
   }

   template<typename Type>
   unsigned int TaskNode<Type>::GetParentCount() const
   {
      return mParentIdList.size();
   }

   template<typename Type>
   void TaskNode<Type>::AddParent(Id aParentId)
   {
      mParentIdList.push_back(aParentId);
   }

   template<typename Type>
   void TaskNode<Type>::RemoveParent(Id aParentId)
   {
      for(std::vector<const Id>::iterator it = mParentIdList.begin(); it != mParentIdList.end(); ++it)
      {
         if((*it) == aParentId)
         {
            mParentIdList.erase(it);
            break;
         }
      }
   }

   template<typename Type>
   ModuleId TaskNode<Type>::GetId() const
   {
      return mId;
   }

   template<typename Type>
   Type& TaskNode<Type>::GetTask()
   {
      return mTask;
   }
}

#endif TASK_NODE_H