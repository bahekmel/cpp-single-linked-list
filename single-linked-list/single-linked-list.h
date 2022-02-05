#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <string>
#include <utility>

// добавьте неоходимые include-директивы сюда

template<typename Type>
class SingleLinkedList {
	struct Node {
		Node() = default;

		Node(const Type& val, Node *next)
				: value(val), next_node(next) {
		}

		Type value;
		Node *next_node = nullptr;
	};

public:

	SingleLinkedList() = default;

	template<typename It>
	void Assign(const It begin, const It end) {
		assert(size_ == 0 && head_.next_node == nullptr);
		SingleLinkedList<Type> tmp1;
		SingleLinkedList<Type> tmp2;
		for (It i = begin; i != end; ++i) {
			tmp1.PushFront(*i);
		}
		for (auto it = tmp1.begin(); it != tmp1.end(); ++it) {
			tmp2.PushFront(*it);
		}
		swap(tmp2);
	}

	SingleLinkedList(std::initializer_list<Type> values) {
		Assign(values.begin(), values.end());
	}

	SingleLinkedList(const SingleLinkedList& other) {
		Assign(other.begin(), other.end());
	}

	SingleLinkedList& operator=(const SingleLinkedList& rhs) {
		if (this != &rhs) {
			SingleLinkedList<Type> tmp(rhs);
			swap(tmp);
		}
		return *this;
	}

	void swap(SingleLinkedList& other) noexcept {
		std::swap(other.head_.next_node, head_.next_node);
		std::swap(other.size_, size_);
	}

	~SingleLinkedList() {
		Clear();
	}

	[[nodiscard]] size_t GetSize() const noexcept {
		return size_;
	}

	[[nodiscard]] bool IsEmpty() const noexcept {
		return size_ == 0;
	}

	void PushFront(const Type& value) {
		head_.next_node = new Node(value, head_.next_node);
		++size_;
	}

	void Clear() noexcept {
		if (size_ > 1) {
			for (int i = 0; i < size_; ++i) {
				if (head_.next_node != nullptr) {
					Node *new_head = head_.next_node->next_node;
					delete head_.next_node;
					head_.next_node = new_head;
				}
			}
			size_ = 0;
		}
	}

private:
	Node head_;
	size_t size_ = 0;


	template<typename ValueType>
	class BasicIterator {
		friend class SingleLinkedList;

		// Конвертирующий конструктор итератора из указателя на узел списка
		explicit BasicIterator(Node *node)
//			assert(false);
				: node_(node) {
		}

	public:
		using iterator_category = std::forward_iterator_tag;
		using value_type = Type;
		using difference_type = std::ptrdiff_t;
		using pointer = ValueType *;
		using reference = ValueType&;

		BasicIterator() = default;

		// Конвертирующий конструктор/конструктор копирования
		// При ValueType, совпадающем с Type, играет роль копирующего конструктора
		// При ValueType, совпадающем с const Type, играет роль конвертирующего конструктора
		BasicIterator(const BasicIterator<Type>& other) noexcept
//			assert(false);
				: node_(other.node_) {
		}

		BasicIterator& operator=(const BasicIterator& rhs) = default;

		// Оператор сравнения итераторов (в роли второго аргумента выступает константный итератор)
		// Два итератора равны, если они ссылаются на один и тот же элемент списка, либо на end()
		[[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
//			assert(node_ != nullptr);
			return this->node_ == rhs.node_;
		}

		// Оператор проверки итераторов на неравенство
		// Противоположен !=
		[[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
//			assert(false);
			return this->node_ != rhs.node_;
		}

		// Оператор сравнения итераторов (в роли второго аргумента итератор)
		// Два итератора равны, если они ссылаются на один и тот же элемент списка, либо на end()
		[[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
//			assert(false);
			return this->node_ == rhs.node_;
		}

		// Оператор проверки итераторов на неравенство
		// Противоположен !=
		[[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
//			assert(false);
			return this->node_ != rhs.node_;
		}

		BasicIterator& operator++() noexcept {
			assert(node_ != nullptr);
			node_ = node_->next_node;
			return *this;
		}

		BasicIterator operator++(int) noexcept {
			assert(node_ != nullptr);
			auto old_value(*this);
			++(*this);
			return old_value;
		}

		[[nodiscard]] reference operator*() const noexcept {
			assert(node_ != nullptr);
			return this->node_->value;
		}

		[[nodiscard]] pointer operator->() const noexcept {
			assert(node_ != nullptr);
			return &node_->value;
		}

	private:
		Node *node_ = nullptr;
	};


public:
	using value_type = Type;
	using reference = value_type&;
	using const_reference = const value_type&;
	using Iterator = BasicIterator<Type>;
	using ConstIterator = BasicIterator<const Type>;

	// Возвращает итератор, ссылающийся на первый элемент
	// Если список пустой, возвращённый итератор будет равен end()
	[[nodiscard]] Iterator begin() noexcept {
//		assert(false);
		return Iterator{head_.next_node};
	}

	// Возвращает итератор, указывающий на позицию, следующую за последним элементом односвязного списка
	// Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
	[[nodiscard]] Iterator end() noexcept {
//		assert(false);
		return Iterator{nullptr};
	}

	// Возвращает константный итератор, ссылающийся на первый элемент
	// Если список пустой, возвращённый итератор будет равен end()
	// Результат вызова эквивалентен вызову метода cbegin()
	[[nodiscard]] ConstIterator begin() const noexcept {
//		assert(false);
		/// Реализуйте самостоятельно
		return cbegin();
	}

	// Возвращает константный итератор, указывающий на позицию, следующую за последним элементом односвязного списка
	// Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
	// Результат вызова эквивалентен вызову метода cend()
	[[nodiscard]] ConstIterator end() const noexcept {
//		assert(false);
		return cend();
	}

	// Возвращает константный итератор, ссылающийся на первый элемент
	// Если список пустой, возвращённый итератор будет равен cend()
	[[nodiscard]] ConstIterator cbegin() const noexcept {
//		assert(false);
		const auto head_cbegin = ConstIterator{head_.next_node};
		return head_cbegin;
	}

	// Возвращает константный итератор, указывающий на позицию, следующую за последним элементом односвязного списка
	// Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
	[[nodiscard]] ConstIterator cend() const noexcept {
//		assert(false);
		const auto head_cend = ConstIterator{nullptr};
		return head_cend;
	}

	// Возвращает итератор, указывающий на позицию перед первым элементом односвязного списка.
	// Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
	[[nodiscard]] Iterator before_begin() noexcept {
		return Iterator{&head_};
	}

	// Возвращает константный итератор, указывающий на позицию перед первым элементом односвязного списка.
	// Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
	[[nodiscard]] ConstIterator cbefore_begin() const noexcept {
		return before_begin();
	}

	// Возвращает константный итератор, указывающий на позицию перед первым элементом односвязного списка.
	// Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
	[[nodiscard]] ConstIterator before_begin() const noexcept {
		return ConstIterator{const_cast<Node *>(&head_)};
	}

	/*
	 * Вставляет элемент value после элемента, на который указывает pos.
	 * Возвращает итератор на вставленный элемент
	 * Если при создании элемента будет выброшено исключение, список останется в прежнем состоянии
	 */
	Iterator InsertAfter(ConstIterator pos, const Type& value) {
		if (pos.node_->next_node == head_.next_node) {
			PushFront(value);
			return Iterator{head_.next_node};
		} else {
			Node *new_one = new Node(value, nullptr);
			Node *save_old = pos.node_;
			new_one->next_node = pos.node_->next_node;
			save_old->next_node = new_one;
			++size_;
			return Iterator{pos.node_->next_node};
		}
	}

	void PopFront() noexcept {
		assert(head_.next_node != nullptr);
		Node *save_head = head_.next_node->next_node;
		delete head_.next_node;
		head_.next_node = save_head;
		--size_;
	}

	/*
	 * Удаляет элемент, следующий за pos.
	 * Возвращает итератор на элемент, следующий за удалённым
	 */
	Iterator EraseAfter(ConstIterator pos) noexcept {
		assert(pos.node_ != nullptr);
		if (pos == before_begin()) {
			PopFront();
		} else {
			Iterator pos_it{pos.node_};
			Node* save_pos = pos.node_->next_node;
			pos_it.node_->next_node = save_pos->next_node;
			delete save_pos;
			--size_;
		}
		return Iterator{pos.node_->next_node};
	}
};

template<typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
lhs.swap(rhs);
}

template<typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
	return (lhs.GetSize() == rhs.GetSize() && std::equal(lhs.begin(), lhs.end(), rhs.begin()));
}

template<typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
	return !(lhs == rhs);
}

template<typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
	return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template<typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
	return lhs < rhs || lhs == rhs;
}

template<typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
	return std::lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(), lhs.end());
}

template<typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
	return lhs > rhs || lhs == rhs;
}
