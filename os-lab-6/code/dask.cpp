#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cmath>

class DiskArm {
public:
    DiskArm();
    ~DiskArm();
    void InitSpace(const char* MethodName); // 使用const char*接收字符串字面量
    void Report();
    void FCFS();
    void SSTF();
    void SCAN();
    void CSCAN();
    void LOOK();
    
private:
    std::vector<int> Request;      // 磁盘请求道号，使用vector替代原始指针
    std::vector<int> Cylinder;     // 工作柱面道号
    int RequestNumber;             // 磁盘请求�?
    int CurrentCylinder;           // 当前道号
    int SeekDirection;             // 磁头方向(0:减小,1:增大)
    int SeekNumber;                // 移臂总数
    int SeekChang;                 // 磁头调头�?
    static const int MaxCylinder;  // 最大柱面号，声明为静态常�?
};

// 静态常量成员的类外定义
const int DiskArm::MaxCylinder = 199;

DiskArm::DiskArm() {
    // 输入当前磁道位置
    std::cout << "Please input current Cylinder:";
    std::cin >> CurrentCylinder;
    
    // 输入磁头方向
    std::cout << "Please input current direction (0/1):";
    std::cin >> SeekDirection;
    
    // 选择请求生成方式
    char choice;
    std::cout << "Generate requests randomly? (y/n):";
    std::cin >> choice;
    
    if (choice == 'y' || choice == 'Y') {
        // 随机生成请求序列
        std::cout << "Please input request numbers:";
        std::cin >> RequestNumber;
        Request.resize(RequestNumber);
        Cylinder.resize(RequestNumber);
        
        srand(time(NULL));
        std::cout << "Random request cylinder string:";
        for (int i = 0; i < RequestNumber; i++) {
            Request[i] = rand() % (MaxCylinder + 1);
            std::cout << Request[i] << " ";
        }
        std::cout << std::endl;
    } else {
        // 手动输入请求序列
        std::cout << "Please input request numbers:";
        std::cin >> RequestNumber;
        std::cout << "Please input request cylinder string:";
        Request.resize(RequestNumber);
        Cylinder.resize(RequestNumber);
        for (int i = 0; i < RequestNumber; i++) {
            std::cin >> Request[i];
        }
    }
}

DiskArm::~DiskArm() {
    // vector会自动释放内存，无需手动delete
}

void DiskArm::InitSpace(const char* MethodName) {
    std::cout << std::endl << MethodName << " Algorithm:" << std::endl;
    SeekNumber = 0;
    SeekChang = 0;
    std::copy(Request.begin(), Request.end(), Cylinder.begin());
}

void DiskArm::Report() {
    std::cout << std::endl;
    std::cout << "Total Seek Distance: " << SeekNumber << std::endl;
    std::cout << "Direction Changes: " << SeekChang << std::endl << std::endl;
}

// 先来先服�?FCFS)算法
void DiskArm::FCFS() {
    int current = CurrentCylinder;
    int direction = SeekDirection;
    InitSpace("FCFS");
    
    std::cout << current;
    for (int i = 0; i < RequestNumber; i++) {
        // 判断是否需要改变方�?
        bool needChange = ((Cylinder[i] >= current && !direction) || 
                          (Cylinder[i] < current && direction));
        if (needChange) {
            direction = !direction;
            SeekChang++;
            std::cout << std::endl << current << "->" << Cylinder[i];
        } else {
            std::cout << "->" << Cylinder[i];
        }
        SeekNumber += abs(current - Cylinder[i]);
        current = Cylinder[i];
    }
    Report();
}

// 最短寻道时间优�?SSTF)算法
void DiskArm::SSTF() {
    InitSpace("SSTF");
    int current = CurrentCylinder;
    int direction = SeekDirection;
    std::vector<int> requests = Cylinder;
    
    std::cout << current;
    while (!requests.empty()) {
        // 寻找最近的磁道
        auto minIt = requests.begin();
        int minDist = abs(*minIt - current);
        
        for (auto it = requests.begin(); it != requests.end(); ++it) {
            int dist = abs(*it - current);
            if (dist < minDist || (dist == minDist && *it < *minIt)) {
                minDist = dist;
                minIt = it;
            }
        }
        
        int next = *minIt;
        requests.erase(minIt);
        
        // 判断方向变化
        int newDirection = (next > current) ? 1 : 0;
        if (current != CurrentCylinder && newDirection != direction) {
            SeekChang++;
            std::cout << std::endl << current << "->" << next;
        } else {
            std::cout << "->" << next;
        }
        
        SeekNumber += minDist;
        current = next;
        direction = newDirection;
    }
    Report();
}

// 扫描(SCAN)算法(电梯算法)
void DiskArm::SCAN() {
    InitSpace("SCAN");
    int current = CurrentCylinder;
    int direction = SeekDirection;
    std::vector<int> lower, higher;
    
    // 分类请求
    for (int cyl : Cylinder) {
        if (cyl < current) lower.push_back(cyl);
        else higher.push_back(cyl);
    }
    
    // 排序：lower降序，higher升序
    std::sort(lower.begin(), lower.end(), std::greater<int>());
    std::sort(higher.begin(), higher.end());
    
    std::cout << current;
    
    if (direction == 1) {  // 增大方向
        // 处理高层请求
        for (int cyl : higher) {
            std::cout << "->" << cyl;
            SeekNumber += abs(current - cyl);
            current = cyl;
        }
        
        // 到达边界后调�?
        if (!lower.empty()) {
            SeekChang++;
            // 移动到最大柱�?
            if (current != MaxCylinder) {
                std::cout << "->" << MaxCylinder;
                SeekNumber += abs(current - MaxCylinder);
                current = MaxCylinder;
            }
            // 处理低层请求
            for (int cyl : lower) {
                std::cout << "->" << cyl;
                SeekNumber += abs(current - cyl);
                current = cyl;
            }
        }
    } else {  // 减小方向
        // 处理低层请求
        for (int cyl : lower) {
            std::cout << "->" << cyl;
            SeekNumber += abs(current - cyl);
            current = cyl;
        }
        
        // 到达边界后调�?
        if (!higher.empty()) {
            SeekChang++;
            // 移动到最小柱�?
            if (current != 0) {
                std::cout << "->0";
                SeekNumber += abs(current - 0);
                current = 0;
            }
            // 处理高层请求
            for (int cyl : higher) {
                std::cout << "->" << cyl;
                SeekNumber += abs(current - cyl);
                current = cyl;
            }
        }
    }
    Report();
}

// 循环扫描(CSCAN)算法
void DiskArm::CSCAN() {
    InitSpace("C-SCAN");
    int current = CurrentCylinder;
    int direction = SeekDirection;
    std::vector<int> lower, higher;
    
    // 分类请求
    for (int cyl : Cylinder) {
        if (cyl < current) lower.push_back(cyl);
        else higher.push_back(cyl);
    }
    
    // 排序：全部升�?
    std::sort(lower.begin(), lower.end());
    std::sort(higher.begin(), higher.end());
    
    std::cout << current;
    
    if (direction == 1) {  // 增大方向
        // 处理高层请求
        for (int cyl : higher) {
            std::cout << "->" << cyl;
            SeekNumber += abs(current - cyl);
            current = cyl;
        }
        
        // 移动到最大柱面并跳转到最小柱�?
        if (!lower.empty()) {
            // 移动到最大柱�?
            if (current != MaxCylinder) {
                std::cout << "->" << MaxCylinder;
                SeekNumber += abs(current - MaxCylinder);
            }
            // 跳转(不计入寻道距离，但显示路�?
            std::cout << "->0";
            current = 0;
            
            // 处理低层请求(按升序，保持增大方向)
            for (int cyl : lower) {
                std::cout << "->" << cyl;
                SeekNumber += abs(current - cyl);
                current = cyl;
            }
        }
    } else {  // 减小方向
        // 处理低层请求(降序排列，保持减小方�?
        std::sort(lower.begin(), lower.end(), std::greater<int>());
        for (int cyl : lower) {
            std::cout << "->" << cyl;
            SeekNumber += abs(current - cyl);
            current = cyl;
        }
        
        // 移动到最小柱面并跳转到最大柱�?
        if (!higher.empty()) {
            // 移动到最小柱�?
            if (current != 0) {
                std::cout << "->0";
                SeekNumber += abs(current - 0);
            }
            // 跳转
            std::cout << "->" << MaxCylinder;
            current = MaxCylinder;
            
            // 处理高层请求(降序排列，保持减小方�?
            std::sort(higher.begin(), higher.end(), std::greater<int>());
            for (int cyl : higher) {
                std::cout << "->" << cyl;
                SeekNumber += abs(current - cyl);
                current = cyl;
            }
        }
    }
    Report();
}

// 查看(LOOK)算法
void DiskArm::LOOK() {
    InitSpace("LOOK");
    int current = CurrentCylinder;
    int direction = SeekDirection;
    std::vector<int> lower, higher;
    
    // 分类请求
    for (int cyl : Cylinder) {
        if (cyl < current) lower.push_back(cyl);
        else higher.push_back(cyl);
    }
    
    // 排序：lower降序，higher升序
    std::sort(lower.begin(), lower.end(), std::greater<int>());
    std::sort(higher.begin(), higher.end());
    
    std::cout << current;
    
    if (direction == 1) {  // 增大方向
        // 处理高层请求(不走到最大柱面，只到最远请�?
        for (int cyl : higher) {
            std::cout << "->" << cyl;
            SeekNumber += abs(current - cyl);
            current = cyl;
        }
        
        // 调头处理低层请求
        if (!lower.empty()) {
            SeekChang++;
            for (int cyl : lower) {
                std::cout << "->" << cyl;
                SeekNumber += abs(current - cyl);
                current = cyl;
            }
        }
    } else {  // 减小方向
        // 处理低层请求(不走到最小柱面，只到最远请�?
        for (int cyl : lower) {
            std::cout << "->" << cyl;
            SeekNumber += abs(current - cyl);
            current = cyl;
        }
        
        // 调头处理高层请求
        if (!higher.empty()) {
            SeekChang++;
            for (int cyl : higher) {
                std::cout << "->" << cyl;
                SeekNumber += abs(current - cyl);
                current = cyl;
            }
        }
    }
    Report();
}

int main() {
    DiskArm disk;
    
    std::cout << "\n===== Disk Scheduling Algorithms Demo =====" << std::endl;
    std::cout << "1. FCFS (First-Come-First-Served)" << std::endl;
    std::cout << "2. SSTF (Shortest Seek Time First)" << std::endl;
    std::cout << "3. SCAN (Elevator Algorithm)" << std::endl;
    std::cout << "4. CSCAN (Circular SCAN)" << std::endl;
    std::cout << "5. LOOK (Limited SCAN)" << std::endl;
    std::cout << "0. Exit" << std::endl;
    
    int choice;
    while (true) {
        std::cout << "Please choose an algorithm (0-5): ";
        std::cin >> choice;
        
        switch (choice) {
            case 1: disk.FCFS(); break;
            case 2: disk.SSTF(); break;
            case 3: disk.SCAN(); break;
            case 4: disk.CSCAN(); break;
            case 5: disk.LOOK(); break;
            case 0: return 0;
            default: std::cout << "Invalid choice, please try again." << std::endl;
        }
    }
    
    return 0;
}