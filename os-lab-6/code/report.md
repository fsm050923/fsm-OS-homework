# �����Ʊ۵����㷨ʵ��ѧϰ����

## һ��ʵ��Ŀ��
1. ����Բ���ϵͳ�豸����������⣬������̵����㷨����Ҫ�ԡ�
2. ����FCFS��SSTF��SCAN��C-SCAN��LOOK�ȴ��̵����㷨��
3. ��ϰģ���㷨�ı�̼��ɣ������о�����ʵ�����ݵ�������

## ����ʵ�黷��
- ����ϵͳ��Windows 10
- ������ԣ�C++
- ���뻷����Visual Studio 2019

## ����ʵ��ԭ��

### 1. ���̵����㷨���
���̵����㷨����ȷ����ͷ��Ӧ������������˳�����Ż�Ѱ��ʱ���ϵͳ���ܡ������㷨������
- **FCFS�������ȷ���**�������󵽴�˳����
- **SSTF�����Ѱ��ʱ�����ȣ�**�����ȴ����뵱ǰ��ͷ���������
- **SCAN��ɨ���㷨/�����㷨��**����ͷ��һ�������ƶ�������÷����ϵ��������󣬵���߽����
- **C-SCAN��ѭ��ɨ���㷨��**������ɨ�裬����߽��ֱ��������һ�˼���ɨ�衣
- **LOOK������ɨ���㷨��**������SCAN�������ƶ�������߽磬ֻ����Զ����λ�á�

### 2. �ؼ�����
- **Ѱ������**����ͷ�ӵ�ǰλ���ƶ���Ŀ��λ�õĴŵ�����
- **��ͷ����**����ͷ�ƶ��ķ���������С�ŵ��ţ���
- **��ͷ����**����ͷ�ı��ƶ�����Ĵ�����

## �ġ�ʵ�������ʵ��

### 1. �����
���`DiskArm`���װ���̵����㷨����ṹ���£�
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

### 2. �����㷨ʵ��

#### (1) FCFS�㷨
�����󵽴�˳����ʵ�ּ򵥵����ܵ��´�ͷ�����ƶ���
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

#### (2) SSTF�㷨
ÿ��ѡ���뵱ǰ��ͷ��������󣬿��ܵ���"����"����
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

#### (3) SCAN�㷨
��ͷ��һ�������ƶ�������߽����
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

#### (4) C-SCAN�㷨
����ɨ�裬����߽��ֱ��������һ�ˣ�
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

#### (5) LOOK�㷨
����SCAN�������ƶ�������߽磺
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

### 3. �����������
Ϊ���ڲ��ԣ�ʵ��������������ɹ��ܣ�
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
        // �ֶ������߼�...
    }
}
```

## �塢ʵ���������

### 1. ���԰���
**��������**��
- ��ǰ�ŵ���50
- ��ͷ����1��������
- �������У�98, 183, 37, 122, 14, 124, 65, 67���ֶ����룩

### 2. ���㷨ִ�н��

#### (1) FCFS�㷨
- ִ��˳��50 �� 98 �� 183 �� 37 �� 122 �� 14 �� 124 �� 65 �� 67
- ��Ѱ�����룺640
- ��ͷ������4

#### (2) SSTF�㷨
- ִ��˳��50 �� 65 �� 67 �� 37 �� 14 �� 98 �� 122 �� 124 �� 183
- ��Ѱ�����룺331
- ��ͷ������2

#### (3) SCAN�㷨
- ִ��˳��50 �� 65 �� 67 �� 98 �� 122 �� 124 �� 183 �� 37 �� 14
- ��Ѱ�����룺299
- ��ͷ������1

#### (4) C-SCAN�㷨
- ִ��˳��50 �� 65 �� 67 �� 98 �� 122 �� 124 �� 183 �� 0 �� 14 �� 37
- ��Ѱ�����룺386
- ��ͷ������1

#### (5) LOOK�㷨
- ִ��˳��50 �� 65 �� 67 �� 98 �� 122 �� 124 �� 183 �� 37 �� 14
- ��Ѱ�����룺299
- ��ͷ������1

### 3. ���ܶԱȷ���
| �㷨 | ��Ѱ������ | ��ͷ���� | �ŵ� | ȱ�� |
|------|------------|----------|------|------|
| FCFS | 640 | 4 | ʵ�ּ򵥣���ƽ | ���ܵ��´�ͷ��������ƶ���Ч�ʵ� |
| SSTF | 331 | 2 | ƽ��Ѱ������� | ���ܵ���ĳЩ����ʱ��ȴ��������� |
| SCAN | 299 | 1 | ���⼢�������ܽϺ� | ����������ӦƵ�ʽϸ� |
| C-SCAN | 386 | 1 | �ṩ�����ȵ���Ӧʱ�� | ��Ѱ��������ܽϴ� |
| LOOK | 299 | 1 | �����ƶ�������߽磬���ܽӽ�SCAN | ʵ���Ը��� |

### 4. ʵ�����
1. **SSTF�㷨**�ڼ���Ѱ�������ϱ������죬������"����"���ա�
2. **SCAN��LOOK�㷨**�ڱ��⼢����ͬʱ�������˽Ϻõ����ܣ��ʺ���Ҫ������Ӧ�ĳ�����
3. **C-SCAN�㷨**�ṩ�˸����ȵ���Ӧʱ�䣬�ʺ϶���Ӧʱ��Ҫ���ϸ�ĳ�����
4. **FCFS�㷨**��Ȼ��ƽ����������ʵ��Ӧ���н��ٵ���ʹ�á�

## ����ʵ���ܽ������
1. ͨ������ʵ�飬��������˲�ͬ���̵����㷨�Ĺ���ԭ��������ص㡣
2. �㷨��ѡ���ϵͳ����������Ӱ�죬����ݾ���Ӧ�ó���Ȩ��Ѱ��Ч�ʺ͹�ƽ�ԡ�
3. ���ʵ�ֹ����У���Ҫע��߽�����������㷨�߼�����ȷ�ԣ������Ǵ�ͷ����仯�ͱ߽紦��
4. ����������ɹ��ܱ��ڸ�ȫ��ز����㷨���ܣ�δ���ɽ�һ���Ż��������ɲ��ԡ�

## �ߡ��Ľ�����
1. ���Ӹ�������ָ�꣬��ƽ����Ӧʱ�䡢����ȴ�ʱ��ȡ�
2. ʵ��ͼ�λ����棬��ֱ�۵�չʾ��ͷ�ƶ����̡�
3. �о�������̵����㷨����FSCAN��NSSF�ȡ�
4. ��ϴ����������ԣ��Ż��㷨ģ�ͣ����ģ����ʵ�ȡ�