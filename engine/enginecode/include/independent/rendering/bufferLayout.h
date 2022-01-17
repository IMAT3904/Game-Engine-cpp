/** \file bufferLayout.h */
#pragma once

#include "shaderDataType.h"
#include <vector>
namespace Engine
{
	/**
	\class VertexBufferElement
	\brief Class wich holds data about a single element in a vertex buffer layout.
	*/
	class VertexBufferElement
	{
	public:
		ShaderDataType m_dataType; //!< Type of data.
		uint32_t m_size; //!< Size of data.
		uint32_t m_offset; //!< Offset in buffer.
		bool m_normalised; //!< Is data normalised? 

		VertexBufferElement() {} //!< Default constructor.
		VertexBufferElement(ShaderDataType dataType, bool normalised = false) :
			m_dataType(dataType),
			m_size(SDT::size(dataType)),
			m_offset(0),
			m_normalised(normalised)
		{} //!< Constructor with parameters.
	};

	/**
	\class UniformBufferElement
	/brief A class wich holds data about a single uniform  buffer layout.
	*/

	class UniformBufferElement
	{
	public:
		const char* m_name; //!< Name of buffer section.
		ShaderDataType m_dataType; //!< Type of data.
		uint32_t m_size; //!< Size of date.
		uint32_t m_offset; //!< Offset in buffer.
		bool m_normalised; //!< Is data normalised.

		UniformBufferElement() {} //!< Default constructor
		UniformBufferElement(const char* name, ShaderDataType dataType) :
			m_name(name),
			m_dataType(dataType),
			m_size(SDT::std140alignment(dataType)),
			m_offset(0)
		{}; //!< Constructor with parameters.
	};
	/** 
	\class BufferLayout
	* \brief Abstraction of the notion of a buffer layout
	*/
	template <class G>
	class BufferLayout
	{
	public:
		BufferLayout<G>() {}; //!< Templated constructor.
		BufferLayout<G>(const std::initializer_list<G>& element, uint32_t stride = 0) : m_elements(element), m_stride(stride) { callStrideAndOffset(); } //!< Templated Constructor with parameters.
		inline uint32_t  getStride() const { return m_stride; } //!< Get width in bytes.
		void addElement(G element); //!< Add an element to buffer.
		inline typename std::vector<G>::iterator begin() { return m_elements.begin(); } //!< Iterator start.
		inline typename std::vector<G>::iterator end() { return m_elements.end(); } //!< Iterator end
		inline typename std::vector<G>::const_iterator begin() const { return m_elements.begin(); } //!< Const version of interator start.
		inline typename std::vector<G>::const_iterator end() const { return m_elements.end(); } //!< Const version of iterator end.
	private:
		std::vector<G> m_elements; //!< Buffer elements
		uint32_t m_stride; //!< width is bytes of the buffer line
		void callStrideAndOffset(); //!< Calculate stride and offsets based on elements
	};

	template <class G>
	void BufferLayout<G>::addElement(G element)
	{
		m_elements.push_back(element);
		callStrideAndOffset();
	}
	template <class G>
	void BufferLayout<G>::callStrideAndOffset()
	{
		uint32_t l_offset = 0;

		for (auto& element : m_elements)
		{
			element.m_offset = l_offset;
			l_offset += element.m_size;
		}
		if(m_stride ==0) m_stride = l_offset;
	}

	using VertexBufferLayout = BufferLayout<VertexBufferElement>;
	using UniformBufferLayout = BufferLayout<UniformBufferElement>;

}