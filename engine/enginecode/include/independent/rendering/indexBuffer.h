/** \file indexBuffer.h */
#pragma once

#include<cstdint>

namespace Engine
{
	/** \class IndexBuffer
	* \brief  API agnostic index bufffer
	*/
	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default; //!< Destructor 
		virtual inline uint32_t getID() const = 0; //!< Get buffer ID
		virtual inline uint32_t getCount() const = 0; //!< Get count.

		static IndexBuffer* create(uint32_t* indicies, uint32_t count); //!< Create buffer
	};
}
