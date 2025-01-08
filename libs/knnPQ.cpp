#include "knnPQ.hpp"
#include <unistd.h>
#include <queue>

using namespace std;


class Quadrant
{
private:
    Point S;
    Point T;

    Point min(Point p1, Point p2)
    {
        return {std::min(p1.x, p2.x), std::min(p1.y, p2.y)};
    }

    Point max(Point p1, Point p2)
    {
        return {std::max(p1.x, p2.x), std::max(p1.y, p2.y)};
    }

public:
    Quadrant() {}

    ~Quadrant() {}

    Quadrant(Point S, Point T)
    {
        this->S = S;
        this->T = T;
    }

    Point getS()
    {
        return S;
    }

    Point getT()
    {
        return T;
    }

    bool Contains(Point point)
    {
        return (S.x <= point.x) && (point.x <= T.x) && (S.y <= point.y) && (point.y <= T.y);
    }

    void printQuad()
    {
        std::cout << "   QUAD: "
                  << "(" << S.x << "," << S.y << ")"
                  << " - "
                  << "(" << T.x << "," << T.y << ")";
    }
};

struct knnpqElement
{

    int position;      // Position of the element in T:L
    Quadrant quadrant; // Quadrant of point
    double distance;      // Mindist
    int level;         // Level in tree of element

    knnpqElement(){};

    ~knnpqElement(){};

    knnpqElement(int _position, Quadrant _quadrant, double _distance, int _level)
    {
        position = _position;
        quadrant = _quadrant;
        distance = _distance;
        level = _level;
    };

    bool operator<(const knnpqElement &other) const
    {
        return this->distance < other.distance;
    }

    void printElement()
    {
        printf("---------ELEMENT---------\n");
        cout << "   POSITION: " << this->position << "\n"
             << "   DISTANCE: " << this->distance << "\n"
             << "   LEVEL: " << this->level << "\n";
        this->quadrant.printQuad();
    }
};

struct KNNObject
{
    Point q;     // Point object containing x, y coordinates and potentially an id (depending on your Point definition)
    double minDist; // Minimum distance to the point q, calculated by minDist

    // Default constructor with empty initialization
    KNNObject() {}

    // Constructor that takes a Point and its minimum distance
    KNNObject(Point q, int minDist) : q(q), minDist(minDist) {}

    // Accessor methods for q and minDist
    Point getQ() const { return q; }
    double getMinDist() const { return minDist; }

    // Function to convert KNNObject to a string (similar to toString() in Java)
    std::string toString() const
    {
        std::string result = "KNNObject{" + std::string("q=") + std::to_string(q.x) + "," + std::to_string(q.y) + ", minDist=" + std::to_string(minDist) + '}';
        return result;
    }
};

class maxHeap
{
public:
    bool operator()(const KNNObject &p1, const KNNObject &p2)
    {
        return p1.minDist < p2.minDist;
    }
};

class minHeap
{
public:
    bool operator()(const knnpqElement &p1, const knnpqElement &p2)
    {
        return p1.distance > p2.distance;
    }
};

double minDist(Point point, Quadrant quadrant)
{
    double R1 = 0;
    double R2 = 0;

    if (quadrant.Contains(point))
    {
        return 0;
    }
    else
    {

        if (point.x < quadrant.getS().x)
        {
            R1 = quadrant.getS().x;
        }
        else if (point.x > quadrant.getT().x)
        {
            R1 = quadrant.getT().x;
        }
        else
        {
            R1 = point.x;
        }

        if (point.y < quadrant.getS().y)
        {
            R2 = quadrant.getS().y;
        }
        else if (point.y > quadrant.getT().y)
        {
            R2 = quadrant.getT().y;
        }
        else
        {
            R2 = point.y;
        }
    }
    double value1 = point.x - R1;
    double value2 = point.y - R2;
    return value1 * value1 + value2 * value2;
}

double minCheb(Point point, Quadrant quadrant)
{
    double R1 = 0;
    double R2 = 0;

    if (quadrant.Contains(point))
    {
        return 0;
    }
    else
    {

        if (point.x < quadrant.getS().x)
        {
            R1 = quadrant.getS().x;
        }
        else if (point.x > quadrant.getT().x)
        {
            R1 = quadrant.getT().x;
        }
        else
        {
            R1 = point.x;
        }

        if (point.y < quadrant.getS().y)
        {
            R2 = quadrant.getS().y;
        }
        else if (point.y > quadrant.getT().y)
        {
            R2 = quadrant.getT().y;
        }
        else
        {
            R2 = point.y;
        }
    }
    return max(abs(point.x - R1),abs(point.y - R2));
}

unsigned long knnPQ::obtenerMemoriaUtilizada()
{
    memoria_utilizada = max_value_pqueue * sizeof(knnpqElement);
    return memoria_utilizada;
}

void  knnPQ::saveMaxPqueue(unsigned long num)
{
    if (num > max_value_pqueue)
    {
        max_value_pqueue = num;
    }
}

knnpqElement getCandidate(bitRankW32Int *btl, Quadrant tmp, long posHijo, Point q, int level)
{
    double minD = minDist(q, tmp);
    return knnpqElement((rank1(btl, posHijo) * (K * K)), tmp, minD, level);
}

bool isCandidate(Quadrant e, priority_queue<KNNObject, vector<KNNObject>, maxHeap>& candidates, int kvecinos, Point q)
{
    double dist = 0;
    double minD = minDist(q, e);
    if (!candidates.empty())
    {
        KNNObject objs = candidates.top();
        dist = objs.getMinDist();
    }
    return (candidates.size() < kvecinos || minD < dist);
}

bool isLeaf(knnpqElement tmp, int ALTURA)
{
    return tmp.level > ALTURA;
}

int getSecuence(knnpqElement tmp)
{
    return (tmp.quadrant.getT().y - tmp.quadrant.getS().y) / K;
}

void evaluateCandidates(knnpqElement tmp, priority_queue<knnpqElement, vector<knnpqElement>, minHeap> &pqueue, priority_queue<KNNObject, vector<KNNObject>, maxHeap>& candidates, int k, Point q, int level, bitRankW32Int *btl)
{
  /*  cout <<"cuadrante padre: ";
    tmp.quadrant.printQuad();
    cout << endl;*/
    int accumX = 0;
    int accumY = 0;
    long posHijo = tmp.position; // en que posición del bitarray tl
    Point S = tmp.quadrant.getS();
    Point T = tmp.quadrant.getT();
    int secuence = getSecuence(tmp);
    Quadrant temp;
    int CANTIDADHIJOS = K * K;
    bool eshoja = 0;

    if (tmp.level == level)
    { // llegue a un cuadrante final...
        accumX = S.x;
        accumY = T.y;
        eshoja = 1;
    }
    else
    {
        accumX = S.x;
        accumY = T.y - secuence;
    }

    for (int i = 1; i <= CANTIDADHIJOS; i++)
    { // por cada hijo de tmp
        if (accumX > T.x)
        { // justo en el limite del cuadrante
            // restele a y y resetee el x
            accumY = accumY - secuence - 1;
            accumX = S.x;
        }
            Point p1;
            p1.x = accumX;
            p1.y = accumY;
            Point p2;
            p2.x = accumX + secuence;
            p2.y= accumY + secuence;
            temp = Quadrant(p1, p2);
      //      temp.printQuad();
        if (isBitSet(btl, posHijo))
        {
     //       std::string leafs= (eshoja==1)? "es hoja":"no es hoja";
       //    cout <<"si esta seteado, pos hijo es: "<< posHijo<< leafs<<endl;
            if (isCandidate(temp, candidates, k, q))
            {
                pqueue.push(getCandidate(btl, temp, posHijo, q, tmp.level + 1));
            }
        } else{
         //   cout <<"NO esta seteado, pos hijo es: "<< posHijo<<endl;

        }// fin if
        accumX = accumX + secuence + 1;
        posHijo++;
    } // fin for
}

Point translateFromAdjacencyMatrix(const Point& p, int numNodes) {
    // Check if p.getY() is valid (within bounds)
    if (p.y< 1 || p.y>numNodes) {
        throw std::invalid_argument("Invalid Y value in adjacency matrix point");
    }

    // Calculate the original Y coordinate
    int y = numNodes - (p.y);

    // Return the Point object with original coordinates and other data
    Point rt;
    rt.x = y;
    rt.y = p.x -1;
    return rt;
}

Point translateToAdjacencyMatrix(const Point& p, int numNodes) {
    Point aux;
    aux.x = p.y +1 ;
    aux.y = numNodes - p.x;
    return aux;
}

vector<pair<int, int>> extractPointsFromCandidates(priority_queue<KNNObject, vector<KNNObject>, maxHeap>& candidates, int numNodes)
{
    vector<pair<int, int>> points;

    // Extract points while candidates queue is not empty
    while (!candidates.empty())
    {
        KNNObject elemento = candidates.top();
        candidates.pop();
        Point aux;
        aux.x = elemento.getQ().x;
        aux.y = elemento.getQ().y;
       Point final = translateFromAdjacencyMatrix(aux, numNodes);
       pair<int, int> cand_point = {final.x, final.y};

        points.push_back(cand_point);
    }

    return points;
}

vector<pair<int, int>> knnPQ::knn(MREP *rep, Point q, int k, int typeDistance)
{
    knnpqElement tmp;
    int dist = -1;
    int ALTURA = rep->maxLevel +1;
   Point pointQuery = translateToAdjacencyMatrix(q,rep->numberOfNodes);

    Point init;
    init.x = 1;
    init.y = 1;
    Point end;
    end.x = rep->numberOfNodes ;
    end.y = rep->numberOfNodes ;

    Quadrant root = Quadrant(init, end);

    priority_queue <knnpqElement, vector<knnpqElement>, minHeap> pqueue; // Min priority queue
    priority_queue <KNNObject, vector<KNNObject>, maxHeap> candidates;   // Max priority queue

    double distinit = (typeDistance == 1) ? minDist(pointQuery, root) : minCheb(pointQuery, root);
    knnpqElement element = knnpqElement(0, root, distinit, 1);

    pqueue.push(element);

    while (pqueue.size() != 0)
    {
        saveMaxPqueue(pqueue.size()); // para saber el maximo ocupado por pqueue y reportar el espacio ocupado..

        if(!pqueue.empty()){
            tmp = pqueue.top();
            pqueue.pop();
        }
        KNNObject obj;
        if(!candidates.empty()){
           obj = candidates.top();
           dist = obj.getMinDist();
        }
        if (candidates.size() == k && tmp.distance >= dist)
        {
            return extractPointsFromCandidates(candidates,rep->numberOfNodes );
        }

        if (isLeaf(tmp, ALTURA))
        {
          //  tmp.printElement();
            KNNObject e2;
            e2.minDist = tmp.distance;
            e2.q = tmp.quadrant.getT();
            if (candidates.size() < k)
            {
                candidates.push(e2);
            }
            else
            {
                if (tmp.distance < candidates.top().getMinDist())
                {
                    candidates.pop();
                    candidates.push(e2);
                }
            }
        }
        else
        {
            // tmp tiene hijos, nodo interno
            evaluateCandidates(tmp, pqueue, candidates, k, pointQuery, ALTURA, rep->btl);

        } // fin else
    } // fin while
    return extractPointsFromCandidates(candidates, rep->numberOfNodes );
}
