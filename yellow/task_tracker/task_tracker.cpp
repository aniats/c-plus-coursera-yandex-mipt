#include <iostream>
#include <map>
#include <algorithm>

using namespace std;

// Перечислимый тип для статуса задачи
enum class TaskStatus {
    NEW,          // новая
    IN_PROGRESS,  // в разработке
    TESTING,      // на тестировании
    DONE          // завершена
};

// Объявляем тип-синоним для map<TaskStatus, int>,
// позволяющего хранить количество задач каждого статуса
using TasksInfo = map<TaskStatus, int>;


class TeamTasks {
public:
    // Получить статистику по статусам задач конкретного разработчика
    const TasksInfo& GetPersonTasksInfo(const string& person) const {
        return person_tasks.at(person);
    }

    // Добавить новую задачу (в статусе NEW) для конкретного разработчитка
    void AddNewTask(const string& person) {
        ++person_tasks[person][TaskStatus::NEW];
    }

    int transfer(int tasks_left, TaskStatus first, TaskStatus second, TasksInfo& old_tasks, TasksInfo& updated_tasks)
    {
        int transferred_tasks = min(old_tasks[first], tasks_left);
        //tasks_left -= transferred_tasks;
        old_tasks[first] -= transferred_tasks;
        updated_tasks[second] = transferred_tasks;

        return tasks_left - transferred_tasks;
    }

    // Обновить статусы по данному количеству задач конкретного разработчика,
    // подробности см. ниже
    tuple<TasksInfo, TasksInfo> PerformPersonTasks(const string& person, int task_count) {
        int tasks_left = task_count;

        TasksInfo old_tasks = person_tasks[person]  ; // GetPersonTasksInfo(person);
        TasksInfo updated_tasks;

        tasks_left = transfer(tasks_left, TaskStatus::NEW, TaskStatus::IN_PROGRESS, old_tasks, updated_tasks);
        tasks_left = transfer(tasks_left, TaskStatus::IN_PROGRESS, TaskStatus::TESTING, old_tasks, updated_tasks);
        tasks_left = transfer(tasks_left, TaskStatus::TESTING, TaskStatus::DONE, old_tasks, updated_tasks);

        person_tasks[person][TaskStatus::NEW] = old_tasks[TaskStatus::NEW] + updated_tasks[TaskStatus::NEW];
        person_tasks[person][TaskStatus::IN_PROGRESS] = old_tasks[TaskStatus::IN_PROGRESS] + updated_tasks[TaskStatus::IN_PROGRESS];
        person_tasks[person][TaskStatus::TESTING] = old_tasks[TaskStatus::TESTING] + updated_tasks[TaskStatus::TESTING];
        person_tasks[person][TaskStatus::DONE] = old_tasks[TaskStatus::DONE] + updated_tasks[TaskStatus::DONE];

        TasksInfo new_upd;
        for (auto it = updated_tasks.begin(); it != updated_tasks.end(); it++) {
            if(it->second != 0) {
                new_upd.insert(pair<TaskStatus, int>(it->first, it->second));
            }
        }

        TasksInfo new_old;
        for (auto it = old_tasks.begin(); it != old_tasks.end(); it++) {
            if(it->second != 0 && it->first != TaskStatus::DONE) {
                new_old.insert(pair<TaskStatus, int>(it->first, it->second));
            }
        }

        return tie(new_upd, new_old);
    }

private:
    map<string, TasksInfo> person_tasks;
};


// Принимаем словарь по значению, чтобы иметь возможность
// обращаться к отсутствующим ключам с помощью [] и получать 0,
// не меняя при этом исходный словарь
void PrintTasksInfo(TasksInfo tasks_info) {
    cout << tasks_info[TaskStatus::NEW] << " new tasks" <<
         ", " << tasks_info[TaskStatus::IN_PROGRESS] << " tasks in progress" <<
         ", " << tasks_info[TaskStatus::TESTING] << " tasks are being tested" <<
         ", " << tasks_info[TaskStatus::DONE] << " tasks are done" << endl;
}

int main() {
    TeamTasks tasks;
    tasks.AddNewTask("Ilia");
    for (int i = 0; i < 3; ++i) {
        tasks.AddNewTask("Ivan");
    }
    cout << "Ilia's tasks: ";
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ilia"));
    cout << "Ivan's tasks: ";
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"));

    TasksInfo updated_tasks, untouched_tasks;

    tie(updated_tasks, untouched_tasks) =
            tasks.PerformPersonTasks("Ivan", 2);
    cout << "Updated Ivan's tasks: ";
    PrintTasksInfo(updated_tasks);
    cout << "Untouched Ivan's tasks: ";
    PrintTasksInfo(untouched_tasks);

    tie(updated_tasks, untouched_tasks) =
            tasks.PerformPersonTasks("Ivan", 2);
    cout << "Updated Ivan's tasks: ";
    PrintTasksInfo(updated_tasks);
    cout << "Untouched Ivan's tasks: ";
    PrintTasksInfo(untouched_tasks);

    return 0;
}
