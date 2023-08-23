#pragma once

#include <memory>
#include <functional>
#include <vector>

#include "Core/Callback.hpp"
#include "Data/Constant.hpp" //#include "Operand.hpp"
#include "Data/BitwiseOperationsUtility.hpp"
#include "Maths/Function.hpp"

namespace ECellEngine::Maths
{
	/*!
	@brief The data struct to describe a mathematical operation.
	@details It is designed for binary operations. A, operation may have other
			 operations as its operands.
	*/
	struct Operation final : public Operand
	{
	public :

		/*!
		@brief The enum helper to build the ::structure of an operation.
		*/
		enum OperationStructure
		{
			OperationStructure_Empty = 0,
			OperationStructure_FirstOperandIsLocal = 1 << 0,
			OperationStructure_SecondOperandIsLocal = 1 << 1,
			OperationStructure_FirstLocalOperandIsOperation = 1 << 2, //it is a Constant if not an Operation
			OperationStructure_FirstLocalOperandIsRHS = 1 << 3, //it is LHS if not RHS
			OperationStructure_SecondLocalOperandIsOperation = 1 << 4, //it is a Constant if not an Operation; it is necessarily RHS since it's the second one
			OperationStructure_IsCompiled = 1 << 5, //infomation flag to know whether ::PushOperands() has been called on this Operation
		};

	private:

		/*!
		@brief Modifies ::structure to indicate that an constant was added.
		@details Use it after ::AddConstant(const float _value).
		*/
		void InformStructureOfAddConstant() noexcept;

		/*!
		@brief Modifies ::structure to indicate that an operation was added.
		@details Use it after ::AddNewOperation(std::string name).
		*/
		void InformStructureOfAddOperation() noexcept;

		/*!
		@brief Decodes ::structure to know in which order operands stored
			   in ::constants or ::operations must be added in ::operands.
		*/
		void PushOperands();

		/*!
		@brief Shrinks the capacity of the vectors ::constants and
				::operations.
		@details This is relevant to save space once we know exactly
				 how many constants or other operations are used as
				 operands of this operation.
		*/
		inline void ShrinkLocalOperands() noexcept
		{
			constants.shrink_to_fit();
			operations.shrink_to_fit();
		}

		/*!
		@brief Updates the pointers in ::operands if needed (i.e. if something
				came from ::constants or ::operations).
		@details It is similar to ::PushOperands but relies on the fact that 
				::operands is already of size 2 to avoid inserts and push_back
				functions. It directly updates using the [] operator at index
				0 (and 1 if needed).
		*/
		void UpdateOperands();

	protected:

		/*!
		@brief The type of the function to perform corresponding to the Operation.
		*/
		FunctionType functionType = FunctionType_Plus;

		/*!
		@brief The function to perform corresponding to the Operation.
		*/
		const Function* function = nullptr;

		/*!
		@brief A byte used to encode how the operation was built.
		@details It is typically usefull if the operation uses other operations
				 or numeric constants as its operands. Indeed, as constants and
				 operations are stored locally in ::constants and ::operations
				 (as opposed to global variables that are stored in the datastate),
				 we need to ascertain which of the two is the left and right operand.
				 In the parsing process, we assume that left operands are added first
				 and the right operands are added last. Therefore, we use 5 bits out
				 of the 8 to describe if one or two local operands are used and which
				 one was it in which order. The relevant bits are grouped as: 765 4 32 10.
				 We do not use the 5th, 6th and 7th bits.

				 Bits 0 and 1 are used to indicate whether one (Bit 0 = 1) or
				 two (Bit 0 = 1 & Bit 1 = 1) local operands are used.

				 Bit 2 indicates whether the 1st local operand is a Constant
				 (Bit 2 = 0) or another operation (Bit 2 = 1).

				 Bit 3 indicates whether the 1st local operand is the left operand
				 (Bit 3 = 0) or the right operand (Bit 3 = 1). This bit is important
				 to detect if a global variable from the datastate has already
				 been added as a left operand to the operation.

				 Bit 4 is equivalent to Bit 2 (distinguish between local
				 constant or another operation) but specifically when a local
				 operand has already been added as the left operand.

				 Bit 5 is to indicate that the Local Operand has already been
				 pushed once in ::operands. It is used to avoid pushing it
				 several times (this happens when an operation is reused to
				 compose another operation).

		@see The source code of ::AddConstant and ::AddOperation for a demonstration
			 of how this byte is written.
		@see The source code of ::PushOperands for a demonstration of how this byte
			 is read.
		see ::OperationStructure to see the enum and flags helpers used to encode
			 and decode this byte.
		*/
		unsigned char structure = 0;

		/*!
		@brief The list of local constants used in the operation.
		*/
		std::vector<ECellEngine::Data::Constant> constants;

		/*!
		@brief The list of local operations used in this operation.
		@details This is equivalent to children operations which builds a tree
				 when taken all together.
		*/
		std::vector<Operation> operations;

		/*!
		@brief The list of right and left operands used in this operation.
		@details It may contain pointers to elements in ::constants, ::operations
				 or another variable in the datastate.
		*/
		std::vector<Operand*> operands;

		/*
		@brief The previous value of the result of the computation of the operation.
		*/
		float previousResult = 0.0f;

		/*!
		@brief The new value of the result of the computation of the operation.
		*/
		float newResult = 0.0f;

	public:

		/*!
		@brief The placeholder for the subscription of ::UpdateLHS to a matching
				callback.
		*/
		std::shared_ptr<std::function<void(const float, const float)>> updateLHSSubToken = nullptr;

		/*!
		@brief The placeholder for the subscription of ::UpdateRHS to a matching
				callback.
		*/
		std::shared_ptr<std::function<void(const float, const float)>> updateRHSSubToken = nullptr;

		/*!
		@brief The callback whenever the value of ::lhs or ::rhs changes.
		*/
		ECellEngine::Core::Callback<const float, const float> onOperandChange;

		/*!
		@brief The callback whenever the value of ::newResult changes.
		*/
		ECellEngine::Core::Callback<const float, const float> onResultChange;

		Operation() : Operand()
		{
			UpdateFunction();//initialize the function pointer to the start value of ::functionType
		}

		Operation(const std::string _name) : 
			Operand (_name)
		{
			constants.reserve(2);
			operations.reserve(2);
			operands.reserve(2);

			UpdateFunction();//initialize the function pointer to the start value of ::functionType
		}

		/*!
		@brief Copy Constructor.
		@details The copy constructor must be specifically declared to maintain the
				 validity of the pointers to Operands in ::operands that come from
				 ::constans and ::operations. Indeed, if this operation is copied
				 to another memory location, a default copy of the pointers in
				 ::operands will lead to the previous location.
		*/
		Operation(const Operation& _op):
			Operand{ _op.name }, structure{ _op.structure }, function{ _op.function },
			constants{_op.constants}, operations{_op.operations}, operands{_op.operands}
		{
			if (Data::Util::IsFlagSet(structure, OperationStructure_IsCompiled))
			{
				UpdateOperands();
			}
		}

		/*!
		@brief Retrieves and fills the list @p _operandsNames with the names of
			   all the operands' names of this operation (and the whole tree).
		@paramt OperandType A specific type of operand to chose from. It MUST
				derivates from ECellEngine::Maths::Operand.
		@param _operandsNames The reference to the vector where to store the
				names of the target operands.
		*/
		template<typename OperandType>//, typename = std::enable_if_t<std::is_base_of_v<Operand, OperandType>>>
		void GetOperandsNames(std::vector<std::string>& _operandsNames) const noexcept
		{
			for (std::vector<Operand*>::const_iterator it = operands.begin(); it != operands.end(); it++)
			{
				if (dynamic_cast<OperandType*>(*it) != nullptr)
				{
					_operandsNames.push_back((*it)->name);
				}
			}

			for (std::vector<Operation>::const_iterator it = operations.begin(); it != operations.end(); it++)
			{
				it->GetOperandsNames<OperandType>(_operandsNames);
			}
		}

		/*!
		@brief Gets the result of this operation.
		@details Uses ::function and ::operands to compute the operation.
		*/
		inline virtual float Get() const noexcept override
		{
			return (*function)(operands);
		}

		/*!
		@brief Returns the reference to the enum representation of the function
				type corresponding to this operation.
		*/
		inline FunctionType& GetFunctionType() noexcept
		{
			return functionType;
		}

		void GetInvolvedEquations(std::vector<std::string>& out_involvedEquations, bool clearOutVector = true) const noexcept override;
		
		void GetInvolvedParameters(std::vector<std::string>& out_involvedParameters, bool clearOutVector = true) const noexcept override;
		
		void GetInvolvedSpecies(std::vector<std::string>& out_involvedSpecies, bool clearOutVector = true) const noexcept override;

		/*!
		@brief DO NOT USE for this class.
		@details Does nothing; simply raises an error in the logger.
		*/
		inline virtual void Set(const float _value) noexcept
		{
			ECellEngine::Logging::Logger::LogError("Operation::Set(const float _value) should not be used.");
		};

		/*!
		@brief Sets the ::function that will be called when computing this
				operation.
		@param _function The pointer to the relevant mathematical function
				to link this operation.
		@param _type The enum representation of the function type corresponding
				to this operation.
		@see ECellEngine::Maths::Function.
		*/
		inline void Set(const Function* _function, FunctionType _type) noexcept
		{
			functionType = _type;
			function = _function;
		}

		/*!
		@brief Updates the pointer in ::function based on ::functionType.
		@details There is a possibility to change ::functionType through
				 the reference returned by ::GetFunctionType. So, we give
				 the possibility to update the function accordingly.
		*/
		void UpdateFunction() noexcept;

		/*!
		@brief Adds a constant in ::constants from its @p _value
		@details Also writes in ::structure to encode this new operand's
				 information.
		*/
		inline ECellEngine::Data::Constant& AddNewConstant(const float _value)
		{
			constants.emplace_back(ECellEngine::Data::Constant(_value));
			InformStructureOfAddConstant();
			return constants.back();
		}

		/*!
		@brief Adds an operation in ::operations from its @p _name
		@details Also writes in ::structure to encode this new operand's
				 information.
		*/
		inline Operation& AddNewOperation(const std::string _name)
		{
			operations.emplace_back(Operation(_name));

			InformStructureOfAddOperation();

			return operations.back();
		}

		/*!
		@brief Adds an operand in ::operands.
		@details Mostly to use when adding a global variable stored in the
				 datastate as an operand in this operation.
		*/
		inline void AddOperand(Operand* _operand)
		{
			operands.push_back(_operand);
		}

		/*!
		@brief Adds an operation in ::operations.
		*/
		inline void AddOperation(Operation& _operation)
		{
			operations.push_back(_operation);
			InformStructureOfAddOperation();
		}

		/*!
		@brief Replaces the pointer to the operand at index @p _idx in
			   ::operands by the pointer @p _operand.
		@param _operand The pointer to the new operand to store.
		@param _idx The index of the operand to replace. MUST be 0 or 1.
		*/
		void OverrideOperand(Operand* _operand, const unsigned char _idx) noexcept;

		/*!
		@brief Checks if there is at least two operands stored in this operation.
		@returns True if the sum of the size of ::operands, ::operations and
				 ::constants is greater or equal to 2.
		*/
		inline bool IsFull()
		{
			return (operands.size() + operations.size() + constants.size() >= 2);
		}

		/*!
		@brief Recursively pushes operands in ::operands and trims unsused
				memory space for every child operation of this operation.
		@details Uses ::PushOperands and ::ShrinkLocalOperands.
		*/
		void LinkLocalOperands();

		inline const std::string ToString() const noexcept override
		{
			return function->ToString(operands);
		}
		
		inline const std::string ToStringValue() const noexcept override
		{
			return '\n' + name + ":" + function->ToStringValue(operands) + "=" + std::to_string((*function)(operands));
		}

		/*!
		@brief Updates the left hand side and triggers the callbacks
				::onOperandChange and ::onResultChange if the corresponding
				conditions are met.
		@details The left hand side is the first operand of this operation.
		*/
		void UpdateLHS(const float _previousValue, const float _newValue) noexcept;

		/*!
		@brief Updates the right hand side and triggers the callbacks
				::onOperandChange and ::onResultChange if the corresponding
				conditions are met.
		@brief The right hand side is the second operand of this operation.
		*/
		void UpdateRHS(const float _previousValue, const float _newValue) noexcept;
	};
}