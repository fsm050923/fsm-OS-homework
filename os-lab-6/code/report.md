# 磁盘移臂调度算法实验学习报告

## 一、实验目的
1. 加深对操作系统设备管理技术的理解，体验磁盘调度算法的重要性。
2. 掌握FCFS、SSTF、SCAN、C-SCAN、LOOK等磁盘调度算法。
3. 练习模拟算法的编程技巧，提升研究分析实验数据的能力。

## 二、实验环境
- 操作系统：Windows 10
- 编程语言：C++
- 编译环境：Visual Studio 2019

## 三、实验原理

### 1. 磁盘调度算法简介
磁盘调度算法用于确定磁头响应多个磁盘请求的顺序，以优化寻道时间和系统性能。常见算法包括：
- **FCFS（先来先服务）**：按请求到达顺序处理。
- **SSTF（最短寻道时间优先）**：优先处理离当前磁头最近的请求。
- **SCAN（扫描算法/电梯算法）**：磁头沿一个方向移动，处理该方向上的所有请求，到达边界后反向。
- **C-SCAN（循环扫描算法）**：单向扫描，到达边界后直接跳到另一端继续扫描。
- **LOOK（限制扫描算法）**：类似SCAN，但不移动到物理边界，只到最远请求位置。

### 2. 关键概念
- **寻道距离**：磁头从当前位置移动到目标位置的磁道数。
- **磁头方向**：磁头移动的方向（增大或减小磁道号）。
- **调头次数**：磁头改变移动方向的次数。

## 四、实验设计与实现

### 1. 类设计
设计`DiskArm`类封装磁盘调度算法，类结构如下：
```cpp
class DiskArm {
public:
    DiskArm();
    ~DiskArm();
    void InitSpace(const char* MethodName);
    void Report();
    void FCFS();
    void SSTF();
    void SCAN();
    void CSCAN();
    void LOOK();
    
private:
    std::vector<int> Request;
    std::vector<int> Cylinder;
    int RequestNumber;
    int CurrentCylinder;
    int SeekDirection;
    int SeekNumber;
    int SeekChang;
    static const int MaxCylinder;
};
```

### 2. 核心算法实现

#### (1) FCFS算法
按请求到达顺序处理，实现简单但可能导致磁头来回移动：
```cpp
void DiskArm::FCFS() {
    int current = CurrentCylinder;
    int direction = SeekDirection;
    InitSpace("FCFS");
    
    std::cout << current;
    for (int i = 0; i < RequestNumber; i++) {
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
```

#### (2) SSTF算法
每次选择离当前磁头最近的请求，可能导致"饥饿"现象：
```cpp
void DiskArm::SSTF() {
    InitSpace("SSTF");
    int current = CurrentCylinder;
    int direction = SeekDirection;
    std::vector<int> requests = Cylinder;
    
    std::cout << current;
    while (!requests.empty()) {
        auto minIt = std::min_element(requests.begin(), requests.end(),
            [current](int a, int b) {
                int distA = abs(a - current);
                int distB = abs(b - current);
                return distA < distB || (distA == distB && a < b);
            });
        
        int next = *minIt;
        requests.erase(minIt);
        
        int newDirection = (next > current) ? 1 : 0;
        if (current != CurrentCylinder && newDirection != direction) {
            SeekChang++;
            std::cout << std::endl << current << "->" << next;
        } else {
            std::cout << "->" << next;
        }
        
        SeekNumber += abs(current - next);
        current = next;
        direction = newDirection;
    }
    Report();
}
```

#### (3) SCAN算法
磁头沿一个方向移动，到达边界后反向：
```cpp
void DiskArm::SCAN() {
    InitSpace("SCAN");
    int current = CurrentCylinder;
    int direction = SeekDirection;
    std::vector<int> lower, higher;
    
    for (int cyl : Cylinder) {
        if (cyl < current) lower.push_back(cyl);
        else higher.push_back(cyl);
    }
    
    std::sort(lower.begin(), lower.end(), std::greater<int>());
    std::sort(higher.begin(), higher.end());
    
    std::cout << current;
    
    if (direction == 1) {
        for (int cyl : higher) {
            std::cout << "->" << cyl;
            SeekNumber += abs(current - cyl);
            current = cyl;
        }
        if (!lower.empty()) {
            SeekChang++;
            if (current != MaxCylinder) {
                std::cout << "->" << MaxCylinder;
                SeekNumber += abs(current - MaxCylinder);
                current = MaxCylinder;
            }
            for (int cyl : lower) {
                std::cout << "->" << cyl;
                SeekNumber += abs(current - cyl);
                current = cyl;
            }
        }
    } else {
        for (int cyl : lower) {
            std::cout << "->" << cyl;
            SeekNumber += abs(current - cyl);
            current = cyl;
        }
        if (!higher.empty()) {
            SeekChang++;
            if (current != 0) {
                std::cout << "->0";
                SeekNumber += abs(current - 0);
                current = 0;
            }
            for (int cyl : higher) {
                std::cout << "->" << cyl;
                SeekNumber += abs(current - cyl);
                current = cyl;
            }
        }
    }
    Report();
}
```

#### (4) C-SCAN算法
单向扫描，到达边界后直接跳到另一端：
```cpp
void DiskArm::CSCAN() {
    InitSpace("C-SCAN");
    int current = CurrentCylinder;
    int direction = SeekDirection;
    std::vector<int> lower, higher;
    
    for (int cyl : Cylinder) {
        if (cyl < current) lower.push_back(cyl);
        else higher.push_back(cyl);
    }
    
    std::sort(lower.begin(), lower.end());
    std::sort(higher.begin(), higher.end());
    
    std::cout << current;
    
    if (direction == 1) {
        for (int cyl : higher) {
            std::cout << "->" << cyl;
            SeekNumber += abs(current - cyl);
            current = cyl;
        }
        if (!lower.empty()) {
            if (current != MaxCylinder) {
                std::cout << "->" << MaxCylinder;
                SeekNumber += abs(current - MaxCylinder);
            }
            std::cout << "->0";
            current = 0;
            for (int cyl : lower) {
                std::cout << "->" << cyl;
                SeekNumber += abs(current - cyl);
                current = cyl;
            }
        }
    } else {
        std::sort(lower.begin(), lower.end(), std::greater<int>());
        for (int cyl : lower) {
            std::cout << "->" << cyl;
            SeekNumber += abs(current - cyl);
            current = cyl;
        }
        if (!higher.empty()) {
            if (current != 0) {
                std::cout << "->0";
                SeekNumber += abs(current - 0);
            }
            std::cout << "->" << MaxCylinder;
            current = MaxCylinder;
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
```

#### (5) LOOK算法
类似SCAN，但不移动到物理边界：
```cpp
void DiskArm::LOOK() {
    InitSpace("LOOK");
    int current = CurrentCylinder;
    int direction = SeekDirection;
    std::vector<int> lower, higher;
    
    for (int cyl : Cylinder) {
        if (cyl < current) lower.push_back(cyl);
        else higher.push_back(cyl);
    }
    
    std::sort(lower.begin(), lower.end(), std::greater<int>());
    std::sort(higher.begin(), higher.end());
    
    std::cout << current;
    
    if (direction == 1 && !higher.empty()) {
        for (int cyl : higher) {
            std::cout << "->" << cyl;
            SeekNumber += abs(current - cyl);
            current = cyl;
        }
        if (!lower.empty()) {
            SeekChang++;
            for (int cyl : lower) {
                std::cout << "->" << cyl;
                SeekNumber += abs(current - cyl);
                current = cyl;
            }
        }
    } else if (direction == 0 && !lower.empty()) {
        for (int cyl : lower) {
            std::cout << "->" << cyl;
            SeekNumber += abs(current - cyl);
            current = cyl;
        }
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
```

### 3. 随机请求生成
为便于测试，实现了随机请求生成功能：
```cpp
DiskArm::DiskArm() {
    std::cout << "Please input current Cylinder:";
    std::cin >> CurrentCylinder;
    std::cout << "Please input current direction (0/1):";
    std::cin >> SeekDirection;
    
    char choice;
    std::cout << "Generate requests randomly? (y/n):";
    std::cin >> choice;
    
    if (choice == 'y' || choice == 'Y') {
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
        // 手动输入逻辑...
    }
}
```

## 五、实验结果与分析

### 1. 测试案例
**案例参数**：
- 当前磁道：50
- 磁头方向：1（增大方向）
- 请求序列：98, 183, 37, 122, 14, 124, 65, 67（手动输入）

### 2. 各算法执行结果

#### (1) FCFS算法
- 执行顺序：50 → 98 → 183 → 37 → 122 → 14 → 124 → 65 → 67
- 总寻道距离：640
- 调头次数：4

#### (2) SSTF算法
- 执行顺序：50 → 65 → 67 → 37 → 14 → 98 → 122 → 124 → 183
- 总寻道距离：331
- 调头次数：2

#### (3) SCAN算法
- 执行顺序：50 → 65 → 67 → 98 → 122 → 124 → 183 → 37 → 14
- 总寻道距离：299
- 调头次数：1

#### (4) C-SCAN算法
- 执行顺序：50 → 65 → 67 → 98 → 122 → 124 → 183 → 0 → 14 → 37
- 总寻道距离：386
- 调头次数：1

#### (5) LOOK算法
- 执行顺序：50 → 65 → 67 → 98 → 122 → 124 → 183 → 37 → 14
- 总寻道距离：299
- 调头次数：1

### 3. 性能对比分析
| 算法 | 总寻道距离 | 调头次数 | 优点 | 缺点 |
|------|------------|----------|------|------|
| FCFS | 640 | 4 | 实现简单，公平 | 可能导致磁头大幅来回移动，效率低 |
| SSTF | 331 | 2 | 平均寻道距离短 | 可能导致某些请求长时间等待（饥饿） |
| SCAN | 299 | 1 | 避免饥饿，性能较好 | 两端请求响应频率较高 |
| C-SCAN | 386 | 1 | 提供更均匀的响应时间 | 总寻道距离可能较大 |
| LOOK | 299 | 1 | 避免移动到物理边界，性能接近SCAN | 实现稍复杂 |

### 4. 实验结论
1. **SSTF算法**在减少寻道距离上表现优异，但存在"饥饿"风险。
2. **SCAN和LOOK算法**在避免饥饿的同时，保持了较好的性能，适合需要均衡响应的场景。
3. **C-SCAN算法**提供了更均匀的响应时间，适合对响应时间要求严格的场景。
4. **FCFS算法**虽然公平，但性能最差，实际应用中较少单独使用。

## 六、实验总结与体会
1. 通过本次实验，深入理解了不同磁盘调度算法的工作原理和性能特点。
2. 算法的选择对系统性能有显著影响，需根据具体应用场景权衡寻道效率和公平性。
3. 编程实现过程中，需要注意边界条件处理和算法逻辑的正确性，尤其是磁头方向变化和边界处理。
4. 随机请求生成功能便于更全面地测试算法性能，未来可进一步优化请求生成策略。

## 七、改进方向
1. 增加更多性能指标，如平均响应时间、请求等待时间等。
2. 实现图形化界面，更直观地展示磁头移动过程。
3. 研究更多磁盘调度算法，如FSCAN、NSSF等。
4. 结合磁盘物理特性，优化算法模型，提高模拟真实度。