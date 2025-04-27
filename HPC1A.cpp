#include <iostream>
#include <stdlib.h>
#include <queue>
#include <omp.h>
using namespace std;

class node
{
public:
    node *left, *right;
    int data;
};

class Breadthfs
{
public:
    node *insert(node *, int);
    void bfs(node *);
};

node *Breadthfs::insert(node *root, int data)
{
    // inserts a node in tree
    if (!root)
    {
        root = new node;
        root->left = NULL;
        root->right = NULL;
        root->data = data;
        return root;
    }

    queue<node *> q;
    q.push(root);

    while (!q.empty())
    {
        node *temp = q.front();
        q.pop();

        if (temp->left == NULL)
        {
            temp->left = new node;
            temp->left->left = NULL;
            temp->left->right = NULL;
            temp->left->data = data;
            return root;
        }
        else
        {
            q.push(temp->left);
        }

        if (temp->right == NULL)
        {
            temp->right = new node;
            temp->right->left = NULL;
            temp->right->right = NULL;
            temp->right->data = data;
            return root;
        }
        else
        {
            q.push(temp->right);
        }
    }

    return root; // Ensure all paths return root
}

void Breadthfs::bfs(node *head)
{
    queue<node *> q;
    q.push(head);

    int qSize;

    while (!q.empty())
    {
        qSize = q.size();

        #pragma omp parallel for
        // creates parallel threads
        for (int i = 0; i < qSize; i++)
        {
            node *currNode;

            #pragma omp critical
            {
                currNode = q.front();
                q.pop();
                cout << "\t" << currNode->data;
            } // prints parent node

            #pragma omp critical
            {
                if (currNode->left) // push parent's left node in queue
                    q.push(currNode->left);
                if (currNode->right) // push parent's right node in queue
                    q.push(currNode->right);
            }
        }
    }
}

int main()
{
    Breadthfs b; // Create an object of Breadthfs class
    node *root = NULL;
    int data;
    char ans;

    do
    {
        cout << "\nEnter data => ";
        cin >> data;

        root = b.insert(root, data); // Use class method to insert

        cout << "Do you want to insert one more node? (y/n): ";
        cin >> ans;

    } while (ans == 'y' || ans == 'Y');

    double start = omp_get_wtime();
    b.bfs(root); // Use class method for BFS
    double end = omp_get_wtime();

    cout << "\nExecution Time: " << (end - start) << " seconds\n";

    return 0;
}
