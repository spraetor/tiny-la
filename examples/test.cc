template <class Model>
struct A {};

template <class Model> // mat_vec_base
struct B
{
	B() {};

	template <class M>
	explicit B(A<M> const&) {}

	template <class M>
	Model& operator=(A<M> const& rhs)
	{
		return static_cast<Model&>(*this);
	}
};

struct C : B<C>, A<C>
{
	typedef C        self;
	typedef B<self>  super;

	self& operator=(self const& rhs)
	{
		return *this;
	}
	using super::operator=;
};

int test()
{
	C c;
	A<C> a;

	c = a;
	return 0;
}