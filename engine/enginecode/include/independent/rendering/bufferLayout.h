/** \file bufferLayout.h */
#pragma once

#include "shaderDataType.h"
#include <vector>
namespace Engine
{
	/**
	\class BufferElement
	A class wich holds data about a single element in a buffer layout
	*/
	class BufferElement
	{
	public:
		ShaderDataType m_dataType;
		uint32_t m_size;
		uint32_t m_offset;
		bool m_normalised;

		BufferElement() {} //!< Default constructor
		BufferElement(ShaderDataType dataType, bool normalised = false) :
			m_dataType(dataType),
			m_size(SDT::size(dataType)),
			m_offset(0),
			m_normalised(normalised)
		{} //!< Constructor with parameters
	};
	/** \class BufferLayout
	*Abstraction of the notion of a buffer layout
	*/
	class BufferLayout
	{
	public:
		BufferLayout() {};
		BufferLayout(const std::initializer_list<BufferElement>& element) : m_elements(element) { callStrideAndOffset(); }
		inline uint32_t  getStride() const { return m_stride; }
		void addElement(BufferElement element);
		inline std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }
		inline std::vector<BufferElement>::iterator end() { return m_elements.end(); }
		inline std::vector<BufferElement>::const_iterator begin() const { return m_elements.begin(); }
		inline std::vector<BufferElement>::const_iterator end() const { return m_elements.end(); }
	private:
		std::vector<BufferElement> m_elements; //!< Buffer elements
		uint32_t m_stride; //!< width is bytes of the buffer line
		void callStrideAndOffset(); //!< Calculate stride and offsets based on elements
	};


}