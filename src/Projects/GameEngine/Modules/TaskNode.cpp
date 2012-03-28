#ifdef TASK_NODE_H

template<typename Type>
TaskNode<Type>::TaskNode()
   : mValue()
   , mId(0)
   , mParentIdList()
   , mParentCount(0)
{
}


template<typename Type>
TaskNode<Type>::TaskNode(const Type& aType)
   : mValue(aType)
   , mId(Incrementator<INCREMENTATOR_ENUM::TASK_ID>::GetIncrement())
   , mParentIdList()
   , mParentCount(0)
{
}

template<typename Type>
TaskNode<Type>::TaskNode(const Type& aType, std::vector<IncrementatorType> aChildrenIdList)
   : mValue(aType)
   , mId(Incrementator<INCREMENTATOR_ENUM::TASK_ID>::GetIncrement())
   , mParentIdList(aChildrenIdList)
   , mParentCount(0)
{
}
template<typename Type>
TaskNode<Type>::TaskNode(const TaskNode<Type>& arTaskNode)
   : mValue(arTaskNode.mValue)
   , mId(arTaskNode.mId)
   , mParentIdList(arTaskNode.mParentIdList)
   , mParentCount(arTaskNode.mParentCount)
{
}

template<typename Type>
TaskNode<Type>::~TaskNode()
{
}

template<typename Type>
unsigned int TaskNode<Type>::GetParentCount() const
{
   return mParentCount;
}

template<typename Type>
void TaskNode<Type>::IncrementParent(IncrementatorType aParentId)
{
   mParentIdList.push_back(aParentId);
   ++mParentCount;
}

template<typename Type>
void TaskNode<Type>::RemoveParent(IncrementatorType aParentId)
{
   for(std::vector<const IncrementatorType>::iterator it = mParentIdList.begin(); it != mParentIdList.end(); ++it)
   {
      if((*it) == aParentId)
      {
         --mParentCount;
         mParentIdList.erase(it);
         break;
      }
   }
}

template<typename Type>
IncrementatorType TaskNode<Type>::GetId() const
{
   return mId;
}

template<typename Type>
Type& TaskNode<Type>::GetValue()
{
   return mValue;
}

#endif TASK_NODE_H