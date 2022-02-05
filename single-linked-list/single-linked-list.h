#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <string>
#include <utility>

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

		explicit BasicIterator(Node *node)
				: node_(node) {
		}

	public:
		using iterator_category = std::forward_iterator_tag;
		using value_type = Type;
		using difference_type = std::ptrdiff_t;
		using pointer = ValueType *;
		using reference = ValueType&;

		BasicIterator() = default;

		BasicIterator(const BasicIterator<Type>& other) noexcept
				: node_(other.node_) {
		}

		BasicIterator& operator=(const BasicIterator& rhs) = default;

		[[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
			return this->node_ == rhs.node_;
		}

		[[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
			return this->node_ != rhs.node_;
		}

		[[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
			return this->node_ == rhs.node_;
		}

		[[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
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

	[[nodiscard]] Iterator begin() noexcept {
		return (size_ == 0) ? end() : Iterator{head_.next_node};
	}

	[[nodiscard]] Iterator end() noexcept {
		return Iterator{nullptr};
	}

	[[nodiscard]] ConstIterator begin() const noexcept {
		return cbegin();
	}

	[[nodiscard]] ConstIterator end() const noexcept {
		return cend();
	}

	[[nodiscard]] ConstIterator cbegin() const noexcept {
		return (size_ == 0) ? cend() : ConstIterator{head_.next_node};
	}

	[[nodiscard]] ConstIterator cend() const noexcept {
		return ConstIterator{nullptr};
	}

	[[nodiscard]] Iterator before_begin() noexcept {
		return Iterator{&head_};
	}

	[[nodiscard]] ConstIterator cbefore_begin() const noexcept {
		return before_begin();
	}

	[[nodiscard]] ConstIterator before_begin() const noexcept {
		return ConstIterator{const_cast<Node *>(&head_)};
	}

	Iterator InsertAfter(ConstIterator pos, const Type& value) {
		if (pos.node_->next_node == head_.next_node) {
			PushFront(value);
			return Iterator{head_.next_node};
		} else {
			Node *new_one = new Node(value, nullptr);
			Node *save_pos = pos.node_;
			new_one->next_node = pos.node_->next_node;
			save_pos->next_node = new_one;
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

	Iterator EraseAfter(ConstIterator pos) noexcept {
		assert(pos.node_ != nullptr);
		if (pos == before_begin()) {
			PopFront();
		} else {
			Iterator it_pos{pos.node_};
			Node *save_pos = pos.node_->next_node;
			it_pos.node_->next_node = save_pos->next_node;
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
