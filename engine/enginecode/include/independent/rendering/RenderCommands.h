/* \file RenderCommands.h */
#pragma once

#include <functional>

namespace Engine
{
	class RendererCommon;
	/** \class RenderCommand
	* \brief API agnostic class for render commands.
	*/
	class RenderCommand
	{
	public:
		enum class Commands{clearDepthBuffer, clearColourBuffer, clearColurAndDepthBuffer,setClearColour,enableDepthTest,disableDepthTest,enableBlending}; //!< List of aviable render commands.
	private:
		std::function<void(void)> m_action; //!< Action of the render command
		friend class RenderCommandFactory; //!< Allow acess to RenderCommandFactory class.
		friend class RendererCommon; //!< Allow acess to RendererCommon class.
	};

	class RenderCommandFactory
	{
	public:
		template <typename ...Args>static RenderCommand* createCommand(RenderCommand::Commands command,Args&& ...args)
		{
			RenderCommand* result = new RenderCommand;
			switch (command)
			{
			case RenderCommand::Commands::clearDepthBuffer:
				result->m_action = clearDepthBufferCommand();
				return result;
				break;
			case RenderCommand::Commands::clearColourBuffer:
				result->m_action = clearColourBufferCommand();
				return result;
				break;
			case RenderCommand::Commands::enableDepthTest:
				result->m_action = enableDepthTestCommand();
				return result;
				break;
			case RenderCommand::Commands::disableDepthTest:
				result->m_action = disableDepthTestCommand();
				return result;
				break;
			case RenderCommand::Commands::enableBlending:
				result->m_action = enableBlendingCommand();
				return result;
				break;
			case RenderCommand::Commands::clearColurAndDepthBuffer:
				result->m_action = getClearColourAndDepthBufferCommand();
				return result;
				break;
			case RenderCommand::Commands::setClearColour:
				float r, g, b, a;

				auto argTuple = std::make_tuple(args...);
				getValue<float, 0>(r, argTuple);
				getValue<float, 1>(g, argTuple);
				getValue<float, 2>(b, argTuple);
				getValue<float, 3>(a, argTuple);


				result->m_action = getsetColourCommand(r, g, b, a);
				return result;
				break;
			}
		}
	private:
		static std::function<void(void)> clearDepthBufferCommand(); //!< Clear Depth buffer.
		static std::function<void(void)> clearColourBufferCommand(); //!< Clear Colour buffer.
		static std::function<void(void)> enableDepthTestCommand(); //!< Enable depth testing. 
		static std::function<void(void)> disableDepthTestCommand(); //!< Disable Depth testing.
		static std::function<void(void)> enableBlendingCommand(); //!< Enable blending.
		static std::function<void(void)> getClearColourAndDepthBufferCommand(); //!< Get clear colour and buffer command.
		static std::function<void(void)> getsetColourCommand(float r, float g, float b, float a); //!< Get set colour command 
		template<typename G,size_t I, typename... Ts>
		typename std::enable_if<I >=sizeof...(Ts),void>::type
		static getValue(G& result, std::tuple<Ts...> tup)
		{
			//If I is great than or equal tuple size don't do eanything
		}
		template <typename G, size_t I, typename... Ts>
		typename std::enable_if<(I < sizeof...(Ts)), void>::type
		static getValue(G& result, std::tuple<Ts...>tup)
		{
			//Get the I thing in the tuple
			result = std::get<I>(tup);
		}
	};
}
