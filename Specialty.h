#pragma once
//
// Specialty.h
// Класс "Специальность" (направление подготовки).
//
// По одной специальности в ВУЗе обучается несколько групп - это основа
// четвёртого запроса варианта "ВУЗ".
//
#include <string>
#include <vector>

class Group;

/**
 * @brief Специальность ВУЗа с её кодом и списком обучающихся групп.
 */
class Specialty {
public:
    /**
     * @brief Создаёт специальность.
     * @param code Код специальности (например, "09.02.07").
     * @param name Название специальности.
     * @throws std::invalid_argument если код или название пустые.
     */
    Specialty(const std::string& code, const std::string& name);

    /** @brief Возвращает код специальности. */
    const std::string& GetCode() const;

    /** @brief Возвращает название специальности. */
    const std::string& GetName() const;

    /**
     * @brief Регистрирует группу, обучающуюся на этой специальности.
     *        Обычно вызывается автоматически конструктором Group -
     *        метод оставлен для явного добавления и проверки дубликатов.
     *        Ссылка не владеющая: группами владеет University.
     * @param group Группа для регистрации.
     * @return true, если группа добавлена; false, если уже была зарегистрирована.
     * @throws std::invalid_argument если @p group равен nullptr.
     */
    bool AddGroup(const Group* group);

    /** @brief Возвращает группы, обучающиеся на этой специальности. */
    const std::vector<Group*>& GetGroups() const;

    /** @brief Возвращает количество обучающихся групп. */
    size_t GetGroupCount() const;

    /** @brief Возвращает краткое представление: "код - название". */
    std::string GetShortInfo() const;

    /**
     * @brief Формирует описание специальности с числом групп.
     * @return Отформатированная строка с описанием.
     */
    std::string GetInfo() const;

private:
    std::string code_;
    std::string name_;
    std::vector<Group*> groups_;
};
