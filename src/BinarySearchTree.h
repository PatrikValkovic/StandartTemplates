#ifndef TEMPLATES_BINARYSEARCHTREE_H
#define TEMPLATES_BINARYSEARCHTREE_H

#include "Vector.h"
#include "Queue.h"
#include "Stack.h"

namespace Templates
{
    template<typename T, bool ALLOW_DUPLICITIES = true>
    class BinarySearchTree
    {
    private:
        class Node
        {
        public:
            T Val;
            Node* Child[2] = {nullptr, nullptr};
        };

        class AssignClass
        {
        private:
            static Node* p;
        public:
            Node*& po;
            Node* working;

            AssignClass() : AssignClass(p, nullptr)
            {}

            AssignClass(Node*& pointer, Node* working)
                    : po(pointer), working(working)
            {}
        };

        Node* Root = nullptr;

        struct TempCopyStructure
        {
            Node* CopyFrom;
            Node** CopyTo;
        };

        int (* Distribution)(const T* const First, const T* const Second);
        int CountOfElements;
    public:
        /**
         * Ditribution - 0 if equal
         *             - <0 if First is smaller then Second
         *             - >0 if First is bigger then Second
         */
        BinarySearchTree(int(* Distribution)(const T* const First, const T* const Second))
        {
            this->CountOfElements = 0;
            this->Distribution = Distribution;
            this->Root = nullptr;
        }

        /**
         * Copy constructor
         */
        BinarySearchTree(BinarySearchTree& Second)
                : BinarySearchTree(Second.Distribution)
        {
            CountOfElements = Second.CountOfElements;

            Queue<TempCopyStructure> CopyQueue;
            TempCopyStructure ToCopy{Second.Root, &this->Root};
            CopyQueue.Push(ToCopy);

            while (!CopyQueue.IsEmpty())
            {
#ifdef ADDITIONAL_TESTS
                if (!CopyQueue.Pop(ToCopy))
                    throw new InternalException(__FILE__, __LINE__);
#else
                CopyQueue.Pop(ToCopy);
#endif
                *ToCopy.CopyTo = new Node;
                (*ToCopy.CopyTo)->Val = ToCopy.CopyFrom->Val;
                for (int a = 0; a < 2; a++)
                    if (ToCopy.CopyFrom->Child[a] != nullptr)
                        CopyQueue.Push(TempCopyStructure{ToCopy.CopyFrom->Child[a],
                                                         (&(*ToCopy.CopyTo)->Child[a])});
            }
        }

        /**
         * Destructor
         */
        ~BinarySearchTree()
        {
            this->Clear();
        }

        BinarySearchTree& operator=(BinarySearchTree& Second)
        {
            if (this == &Second)
                return *this;

            this->Clear();
            this->BinarySearchTree(Second);
            return *this;
        }

        /**
         * Return count of elements in tree
         */
        int Size()
        {
            return this->CountOfElements;
        }

        /**
         * Insert element into tree
         * return 1, if was element inserted
         */
        int Insert(T& Value)
        {
            Node* Created = new Node;
            Created->Val = Value;
            if (Root == nullptr)
            {
                this->Root = Created;
                CountOfElements++;
                return 1;
            }

            Node* Previous = nullptr;
            Node* Temp = this->Root;
            while (Temp != nullptr)
            {
                int Result = this->Distribution(&Temp->Val, &Value);
                Previous = Temp;
                if (!ALLOW_DUPLICITIES && Result == 0)
                {
                    delete Created;
                    return 0;
                }
                Temp = Temp->Child[Result >= 0];
            }
            Previous->Child[this->Distribution(&Previous->Val, &Value) >= 0] = Created;
            CountOfElements++;
            return 1;
        }

        /**
         * Insert @Count values into tree
         * returns count of inserted elements
         */
        int Insert(T* Values, int Count)
        {
            int inserted = 0;
            for (int a = 0; a < Count; a++)
                inserted += this->Insert(*(Values++));
            return inserted;
        }

        /**
         * Removes all nodes from tree
         * Return count of deleted nodes
         */
        int Clear()
        {
            if (this->Root == nullptr)
                return 0;
            int Deleted = 0;
            Stack<Node*>* ToDeleteStack = new Stack<Node*>();
            ToDeleteStack->Push(this->Root);

            Node* ToDelete;
            while (!ToDeleteStack->IsEmpty())
            {
                if (!ToDeleteStack->Pop(ToDelete))
                {
                    delete ToDeleteStack;
                    return -1;
                }

                if (ToDelete->Child[1])
                    ToDeleteStack->Push(ToDelete->Child[1]);
                if (ToDelete->Child[0])
                    ToDeleteStack->Push(ToDelete->Child[0]);
                delete ToDelete;
                Deleted++;
            }
            this->Root = nullptr;
            delete ToDeleteStack;
            CountOfElements -= Deleted;
            return Deleted;
        }

        /**
         * Return true, if is Value in tree, otherwise false
         */
        bool Contain(const T& Value) const
        {
            if (this->Root == nullptr)
                return false;
            Node* Temp = this->Root;
            while (Temp != nullptr)
            {
                int Resolve = this->Distribution(&Temp->Val, &Value);
                if (Resolve == 0)
                    return true;
                Temp = Temp->Child[Resolve > 0];
            }
            return false;
        }

        /**
         * Return element with specific properties
         * Return 1 on success, 0 otherwise
         */
        int Get(const T& ToFind, T*& Returned) const
        {
            Node* Result = this->GetNode(ToFind);
            if (Result == nullptr)
                return 0;
            Returned = &(Result->Val);
            return 1;
        }

        /**
         * Return element by callback
         */
        Vector <T> Get(bool(* Callback)(const T* const Value, void* data), void* data = nullptr)
        {
            if (this->Root == nullptr)
                return Vector<T>();

            Vector<T> vals;
            Stack<Node*> rest;
            Node* current = this->Root;

            while (current != nullptr)
            {
                rest.Push(current);
                current = current->Child[0];
            }

            while (!rest.IsEmpty())
            {
                Node* ToProccess;
                rest.Pop(ToProccess);
                if (Callback(&ToProccess->Val, data))
                    vals.Insert(ToProccess->Val);
                if (ToProccess->Child[1] != nullptr)
                {
                    current = ToProccess->Child[1];
                    while (current != nullptr)
                    {
                        rest.Push(current);
                        current = current->Child[0];
                    }
                }
            }

            return vals;
        }

        /**
         * Try to find specific element
         * If this element doesnt exists, creates new based on parameter
         * Return -1 if fails, 0 if success and 1 if was created
         */
        int GetOrCreate(T& ToFind, T*& Return)
        {
            Node* Result = this->GetNode(ToFind);

            if (Result != nullptr)
            {
                Return = &(Result->Val);
                return 0;
            }

#ifdef ADDITIONAL_TESTS
            int Inserted = this->Insert(ToFind);
            if (Inserted != 1)
                throw new InternalException(__FILE__, __LINE__);
            Inserted = this->Get(ToFind, Return);
            if (Inserted)
                return 1;
            else
                throw new InternalException(__FILE__, __LINE__);
#else
            this->Insert(ToFind);
            this->Get(ToFind, Return);
#endif
            return 1;
        }

        /**
         * Try to find specific element, remove it and return it
         * Return 0 if wasnt found, 1 if was element deleted
         */
        int GetAndDelete(T& ToFind, T& Return)
        {
            Node* Returned = this->GetNode(ToFind);
            if (Returned == nullptr)
                return 0;
            Return = Returned->Val;
            this->Delete(Return);
            return 1;
        }

    private:
        void ToArrayRecursive(Vector <T>& Array, Node* Temp) const
        {
            if (Temp == nullptr)
                return;
            ToArrayRecursive(Array,
                             Temp->Child[1]); //must be reversed, because new elements are inserted to begin of vector
            Array.Insert(Temp->Val);
            ToArrayRecursive(Array, Temp->Child[0]);
            return;
        }

        class ToDeleteHelpClass
        {
        public:
            ToDeleteHelpClass()
            {}

            ToDeleteHelpClass(Node* Parent, Node* ToDelete)
                    : Parent(Parent), ToDelete(ToDelete)
            {}

            Node* Parent = nullptr;
            Node* ToDelete = nullptr;
        };

        /**
         * Remove node from tree.
         * Return node replaced by original node, nullptr if was leaf
         */
        Node* RemoveNode(ToDeleteHelpClass Deleting)
        {
            Node** ParentPointer = nullptr;
            if (Deleting.ToDelete == this->Root)
                ParentPointer = &this->Root;
            else
                ParentPointer = &Deleting.Parent->Child[(Deleting.Parent->Child[1] == Deleting.ToDelete)];

            //ToDelete is leaf
            if (Deleting.ToDelete->Child[0] == nullptr && Deleting.ToDelete->Child[1] == nullptr)
            {
                delete Deleting.ToDelete;
                *ParentPointer = nullptr;
                return nullptr;
            }

            //if ToDelete have only one children
            if (Deleting.ToDelete->Child[0] == nullptr || Deleting.ToDelete->Child[1] == nullptr)
            {
                *ParentPointer = Deleting.ToDelete->Child[Deleting.ToDelete->Child[0] == nullptr];
                delete Deleting.ToDelete;
                return *ParentPointer;
            }

            //just rotate
            if (Deleting.ToDelete->Child[0]->Child[1] == nullptr)
            {
                Deleting.ToDelete->Child[0]->Child[1] = Deleting.ToDelete->Child[1];
                *ParentPointer = Deleting.ToDelete->Child[0];
                delete Deleting.ToDelete;
                return *ParentPointer;
            }
            if (Deleting.ToDelete->Child[1]->Child[0] == nullptr)
            {
                Deleting.ToDelete->Child[1]->Child[0] = Deleting.ToDelete->Child[0];
                *ParentPointer = Deleting.ToDelete->Child[1];
                delete Deleting.ToDelete;
                return *ParentPointer;
            }

            Node* Parent = Deleting.ToDelete->Child[0];
            Node* Replaced = Parent->Child[1];
            while (Replaced->Child[1] != nullptr)
            {
                Parent = Replaced;
                Replaced = Parent->Child[1];
            }
            Parent->Child[1] = Replaced->Child[0];
            for (int a = 0; a < 2; a++)
                Replaced->Child[a] = Deleting.ToDelete->Child[a];
            *ParentPointer = Replaced;
            delete Deleting.ToDelete;
            return *ParentPointer;
        }

        Node* GetNode(const T& ToFind) const
        {
            if (this->Root == nullptr)
                return nullptr;
            Node* Temp = this->Root;
            while (Temp != nullptr)
            {
                int Resolve = this->Distribution(&Temp->Val, &ToFind);
                if (Resolve == 0)
                {
                    return Temp;
                }
                Temp = Temp->Child[Resolve > 0];
            }
            return nullptr;
        }

    public:
        /**
         * Return all elements in tree.
         * Elements will be sorted
         * Otherwise nullptr
         */
        T* ToArray(int& Count) const
        {
            Vector<T> ArrayTemp;
            ToArrayRecursive(ArrayTemp, this->Root);
            Array<T> arr = ArrayTemp.ToArray();
            T* array = new T[arr.Size()];
            Count = arr.Size();
            for(int i=0;i<Count;i++)
                array[i] = arr[i];
            return array;
        }

        /**
         * Delete all elements, in which callback return true
         * Return count of deleted elements
         * data parameter will be passed to callback
         */
        int Delete(bool(* Callback)(const T* const Value, void* data), void* data = nullptr)
        {
            if (this->Root == nullptr)
                return 0;
            int deleted = 0;
            Queue<ToDeleteHelpClass> Nodes;
            Nodes.Push(ToDeleteHelpClass(nullptr, this->Root));

            ToDeleteHelpClass Temp;
            while (Nodes.Pop(Temp))
            {
                if (Callback(&Temp.ToDelete->Val, data))
                {
                    Node* Replaced = this->RemoveNode(Temp);
                    if (Replaced != nullptr)
                        Nodes.Push(ToDeleteHelpClass(Temp.Parent, Replaced));
                    deleted++;
                }
                else
                {
                    for (int a = 0; a < 2; a++)
                        if (Temp.ToDelete->Child[a] != nullptr)
                            Nodes.Push(ToDeleteHelpClass(Temp.ToDelete, Temp.ToDelete->Child[a]));
                }
            }
            CountOfElements -= deleted;
            return deleted;
        }

        /**
         * Delete item from tree
         * return 1 if success, 0 otherwise
         */
        int Delete(T value)
        {
            if (this->Root == nullptr)
                return 0;

            Node* Temp = this->Root;
            Node* Parent = nullptr;

            while (Temp != nullptr)
            {
                int Resolve = this->Distribution(&Temp->Val, &value);
                if (Resolve == 0)
                    break;
                Parent = Temp;
                Temp = Temp->Child[Resolve > 0];
            }

            if (Temp == nullptr)
                return 0;

            this->RemoveNode(ToDeleteHelpClass(Parent, Temp));
            CountOfElements--;
            return 1;
        }
    };
}
#endif //TEMPLATES_BINARYSEARCHTREE_H
