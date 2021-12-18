#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

class Element;
class Number;
class Pair;

class Element
{
public:
	virtual ~Element() {}
	Pair* Outer;

	static std::shared_ptr<Pair> Add(std::shared_ptr<Element> First, std::shared_ptr<Element> Second);
	bool ReduceExplode(std::size_t NestedCount = 0);
	bool ReduceSplit();
	bool Reduce();
	void Print();

	std::size_t Magnitude();

	std::shared_ptr<Element> MakeCopy();
};

class Number : public Element
{
public:
	Number(std::size_t NewValue) : Value(NewValue) {}

	virtual ~Number() override {}
	std::size_t Value;

	void Split();
};

class Pair : public Element
{
public:
	virtual ~Pair() override {}
	std::shared_ptr<Element> Left;
	std::shared_ptr<Element> Right;

	void Explode();
};

std::size_t Element::Magnitude()
{
	if (Pair* CurrentPair = dynamic_cast<Pair*>(this))
	{
		return (CurrentPair->Left->Magnitude() * 3) + (CurrentPair->Right->Magnitude() * 2);
	}
	else if (Number* CurrentNumber = dynamic_cast<Number*>(this))
	{
		return CurrentNumber->Value;
	}
	throw;
}

std::shared_ptr<Element> Element::MakeCopy()
{
	if (Pair* CurrentPair = dynamic_cast<Pair*>(this))
	{
		std::shared_ptr<Pair> NewPair = std::make_shared<Pair>();
		NewPair->Left = CurrentPair->Left->MakeCopy();
		NewPair->Right = CurrentPair->Right->MakeCopy();
		NewPair->Outer = CurrentPair->Outer;
		NewPair->Left->Outer = NewPair.get();
		NewPair->Right->Outer = NewPair.get();
		return NewPair;
	}
	else if (Number* CurrentNumber = dynamic_cast<Number*>(this))
	{
		std::shared_ptr<Number> NewNumber = std::make_shared<Number>(CurrentNumber->Value);
		NewNumber->Outer = CurrentNumber->Outer;
		return NewNumber;
	}
	throw;
}

void Element::Print()
{
	if (Pair* TargetPair = dynamic_cast<Pair*>(this))
	{
		std::cout << '[';
		TargetPair->Left->Print();
		std::cout << ',';
		TargetPair->Right->Print();
		std::cout << ']';
	}
	else if (Number* TargetNumber = dynamic_cast<Number*>(this))
	{
		std::cout << TargetNumber->Value;
	}
	else
	{
		throw;
	}
}


void Number::Split()
{
	if (Outer)
	{
		std::shared_ptr<Pair> NewPair = std::make_shared<Pair>();
		NewPair->Left = std::make_shared<Number>(Value / 2);
		NewPair->Right = std::make_shared<Number>((Value + 1) / 2);
		NewPair->Outer = Outer;
		NewPair->Left->Outer = NewPair.get();
		NewPair->Right->Outer = NewPair.get();

		if (Outer->Left.get() == this)
		{
			Outer->Left = std::move(NewPair);
			return;
		}
		else if (Outer->Right.get() == this)
		{
			Outer->Right = std::move(NewPair);
			return;
		}
	}
	throw;
}

void Pair::Explode()
{
	Number& LeftNumber = dynamic_cast<Number&>(*Left);
	Number& RightNumber = dynamic_cast<Number&>(*Right);

	// Get leftmost number
	Pair* LoopOuter = Outer;
	Pair* BiggerOuter = this;
	while (LoopOuter)
	{
		if (LoopOuter == BiggerOuter->Outer)
		{
			if (LoopOuter->Left.get() != BiggerOuter)
			{
				if (Pair* OuterPairLeftPair = dynamic_cast<Pair*>(LoopOuter->Left.get()))
				{
					LoopOuter = OuterPairLeftPair;
					continue;
				}
				else if (Number* OuterPairLeftNumber = dynamic_cast<Number*>(LoopOuter->Left.get()))
				{
					OuterPairLeftNumber->Value += LeftNumber.Value;
					break;
				}
			}
		}
		else
		{
			if (Pair* OuterPairRightPair = dynamic_cast<Pair*>(LoopOuter->Right.get()))
			{
				LoopOuter = OuterPairRightPair;
				continue;
			}
			else if (Number* OuterPairRightNumber = dynamic_cast<Number*>(LoopOuter->Right.get()))
			{
				OuterPairRightNumber->Value += LeftNumber.Value;
				break;
			}
		}

		LoopOuter = LoopOuter->Outer;
		BiggerOuter = BiggerOuter->Outer;
	}

	// Get the rightmost number
	LoopOuter = Outer;
	BiggerOuter = this;
	while(LoopOuter)
	{
		if (LoopOuter == BiggerOuter->Outer)
		{
			if (LoopOuter->Right.get() != BiggerOuter)
			{
				if (Pair* OuterPairRightPair = dynamic_cast<Pair*>(LoopOuter->Right.get()))
				{
					LoopOuter = OuterPairRightPair;
					continue;
				}
				else if (Number* OuterPairRightNumber = dynamic_cast<Number*>(LoopOuter->Right.get()))
				{
					OuterPairRightNumber->Value += RightNumber.Value;
					break;
				}
			}
		}
		else 
		{
			if (Pair* OuterPairLeftPair = dynamic_cast<Pair*>(LoopOuter->Left.get()))
			{
				LoopOuter = OuterPairLeftPair;
				continue;
			}
			else if (Number* OuterPairLeftNumber = dynamic_cast<Number*>(LoopOuter->Left.get()))
			{
				OuterPairLeftNumber->Value += RightNumber.Value;
				break;
			}
		}
		
		LoopOuter = LoopOuter->Outer;
		BiggerOuter = BiggerOuter->Outer;
	}

	if (Outer)
	{
		std::shared_ptr<Number> NewNumber = std::make_shared<Number>(0);
		NewNumber->Outer = Outer;
		if (Outer->Left.get() == this)
		{
			Outer->Left = std::move(NewNumber);
			return;
		}
		else if (Outer->Right.get() == this)
		{
			Outer->Right = std::move(NewNumber);
			return;
		}
	}
	throw;
}

std::shared_ptr<Pair> Element::Add(std::shared_ptr<Element> First, std::shared_ptr<Element> Second)
{
	std::shared_ptr<Pair> NewPair = std::make_shared<Pair>();
	std::shared_ptr<Element> LeftElement = First->MakeCopy();
	std::shared_ptr<Element> RightElement = Second->MakeCopy();
	LeftElement->Outer = NewPair.get();
	RightElement->Outer = NewPair.get();
	NewPair->Left = LeftElement;
	NewPair->Right = RightElement;
	NewPair->Reduce();
	return NewPair;
}

bool Element::ReduceExplode(std::size_t NestedCount)
{
	if (Pair* CurrentPair = dynamic_cast<Pair*>(this))
	{
		if (NestedCount == 4)
		{
			CurrentPair->Explode();
			return true;
		}
		if (CurrentPair->Left->ReduceExplode(NestedCount + 1))
		{
			return true;
		}
		if (CurrentPair->Right->ReduceExplode(NestedCount + 1))
		{
			return true;
		}
	}
	return false;
}

bool Element::ReduceSplit()
{
	if (Number* CurrentNumber = dynamic_cast<Number*>(this))
	{
		if (CurrentNumber->Value >= 10)
		{
			CurrentNumber->Split();
			return true;
		}
	}
	else if (Pair* CurrentPair = dynamic_cast<Pair*>(this))
	{
		if (CurrentPair->Left->ReduceSplit())
		{
			return true;
		}
		if (CurrentPair->Right->ReduceSplit())
		{
			return true;
		}
	}
	return false;
}

bool Element::Reduce()
{
	while (true)
	{
		if (ReduceExplode())
		{
			continue;
		}
		if (ReduceSplit())
		{
			continue;
		}
		break;
	}
	return true;
}

int main()
{
	std::ifstream Input("input.txt");

	std::size_t NumberBuffer;

	std::shared_ptr<Element> Outermost;
	Pair* CurrentOuter = nullptr;

	std::vector<std::shared_ptr<Element>> Elements;

	while (Input)
	{
		const char PeekedChar = Input.peek();
		if (PeekedChar == '[')
		{
			std::shared_ptr<Pair> NewPair = std::make_shared<Pair>();
			NewPair->Outer = CurrentOuter;
			CurrentOuter = NewPair.get();
			if (!Outermost)
			{
				Outermost = std::move(NewPair);
			}
			else if (NewPair->Outer)
			{
				if (!NewPair->Outer->Left)
				{
					NewPair->Outer->Left = std::move(NewPair);
				}
				else if (!NewPair->Outer->Right)
				{
					NewPair->Outer->Right = std::move(NewPair);
				}
				else
				{
					throw;
				}
			}
			else
			{
				throw;
			}
		}
		else if (PeekedChar == ']')
		{
			CurrentOuter = CurrentOuter->Outer;
		}
		else if (std::isdigit(PeekedChar))
		{
			Input >> NumberBuffer;
			std::shared_ptr<Number> NewNumber = std::make_shared<Number>(NumberBuffer);
			NewNumber->Outer = CurrentOuter;
			if (!Outermost)
			{
				Outermost = std::move(NewNumber);
			}
			if (CurrentOuter)
			{
				if (!CurrentOuter->Left)
				{
					CurrentOuter->Left = std::move(NewNumber);
				}
				else if (!CurrentOuter->Right)
				{
					CurrentOuter->Right = std::move(NewNumber);
				}
				else
				{
					throw;
				}
			}
			continue;
		}
		else if (PeekedChar == '\n')
		{
			Elements.push_back(std::move(Outermost));
			Outermost.reset();
			CurrentOuter = nullptr;
		}

		Input.ignore(1);
	}

	std::size_t HighestMagnitude = 0;
	for (auto& FirstElement : Elements)
	{
		for (auto& SecondElement : Elements)
		{
			HighestMagnitude = std::max(HighestMagnitude, Element::Add(FirstElement, SecondElement)->Magnitude());
			HighestMagnitude = std::max(HighestMagnitude, Element::Add(SecondElement, FirstElement)->Magnitude());
		}
	}

	std::cout << HighestMagnitude << std::endl;
}