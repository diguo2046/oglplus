/*
 *  Copyright 2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_buffer_common_1
namespace oglplus {

template <>
class __ObjCommonOps<__tag_Buffer>
 : public __BufferName
{
public:
	typedef __BufferTarget Target; /*<
	Buffer bind targets.
	>*/
	typedef __BufferIndexedTarget IndexedTarget;

	static __BufferName Binding(Target target); /*<
	Returns the buffer currently bound to the specified [^target].
	>*/
	static void Bind(Target target, __BufferName buffer); /*<
	Binds the specified [^buffer] to the specified [^target].
	>*/

	static __BufferName Binding(IndexedTarget target, GLuint index); /*<
	Returns the current buffer bound to the specified [^index]ed [^target].
	>*/
	static void BindBase(
		IndexedTarget target,
		GLuint index,
		__BufferName buffer
	); /*<
	Binds the specified [^buffer] to the specified [^index]ed [^target].
	>*/
	static void BindRange(
		IndexedTarget target,
		GLuint index,
		__BufferName buffer,
		__BufferSize offset,
		__BufferSize size
	); /*<
	Bind a range starting at [^offset] with length specified by [^size]
	in a [^buffer] to the specified [^index]ed [^target].
	>*/

#if GL_VERSION_4_4 || GL_ARB_multi_bind
	static void BindBase(
		IndexedTarget target,
		GLuint first,
		const __Sequence<__BufferName>& buffers
	); /*<
	Sequentially binds all [^buffers] to indexed [^target]s starting at
	[^first] index.
	>*/

	static void BindRange(
		IndexedTarget target,
		GLuint first,
		const __Sequence<__BufferName>& buffers,
		const GLintptr* offsets,
		const GLsizeiptr* sizes
	); /*<
	Sequentially binds ranges (specified by [^offsets] and [^sizes] which must
	point to arrays of at least as much values as the [^buffers] sequence)
	of all [^buffers] to indexed [^target]s starting at [^first] index.
	>*/
#endif
//]
//[oglplus_buffer_common_2
	void Bind(Target target) const; /*<
	Binds [^this] buffer to the specified [^target].
	>*/
	void Bind(IndexedTarget target, GLuint index) const; /*<
	Binds [^this] buffer to the specified indexed [^target].
	>*/
	void BindBase(IndexedTarget target, GLuint index) const;

#if GL_VERSION_4_0 || GL_ARB_transform_feedback3
	void BindBaseUniform(__UniformBufferBindingPoint index) const; /*<
	Binds [^this] buffer to the uniform buffer binding point with
	the specified [^index].
	>*/
#endif

#if GL_VERSION_4_0 || GL_ARB_transform_feedback3
	void BindBaseTransformFeedback(__TransformFeedbackBufferBindingPoint index) const /*<
	Binds [^this] buffer to the transform feedback binding point with
	the specified [^index].
	>*/
#endif

#if GL_VERSION_4_2 || GL_ARB_shader_atomic_counters
	void BindBaseAtomicCounter(__AtomicCounterBufferBindingPoint index) const; /*<
	Binds [^this] buffer to the atomic counter binding point with
	the specified [^index].
	>*/
#endif

#if GL_VERSION_4_3 || GL_ARB_shader_storage_buffer_object
	void BindBaseShaderStorage(__ShaderStorageBufferBindingPoint index) const; /*<
	Binds [^this] buffer to the shader storage binding point with
	the specified [^index].
	>*/
#endif

	void BindRange(
		IndexedTarget target,
		GLuint index,
		__BufferSize offset,
		__BufferSize size
	) const; /*<
	Binds a range (specified by [^offset] and [^size]) of [^this] buffer
	to the specified [^index]ed [^target].
	>*/
//]
//[oglplus_buffer_common_3
#if GL_VERSION_4_3 || GL_ARB_invalidate_subdata
	void InvalidateData(void); /*<
	Invalidate all of [^this] buffer's data.
	See [glfunc InvalidateBufferData].
	>*/
	void InvalidateSubData(__BufferSize offset, __BufferSize size); /*<
	Invalidate a subrange (specified by [^offset] and [^size])
	of [^this] buffer's data.
	See [glfunc InvalidateBufferSubData].
	>*/
#endif
};
//]
//[oglplus_buffer_zero
template <>
class __ObjZeroOps<__tag_ExplicitSel, __tag_Buffer>
 : public __ObjCommonOps<__tag_Buffer>
{
public: /*<
There are no operations that can work explicitly on buffer
object zero besides binding, so this class just inherits from
the common buffer operations wrapper.
>*/
};

//]
//[oglplus_buffer_1
template <>
class __ObjectOps<__tag_ExplicitSel, __tag_Buffer>
 : public __ObjZeroOps<__tag_ExplicitSel, __tag_Buffer>
{
public:
	struct Property
	{
		typedef __BufferUsage Usage;
		typedef __BufferMapAccess MapAccess;
	};

	typedef __BufferTypedMap<GLubyte> Map; /*<
	Mapping of the buffer to the client address space.
	>*/

	static bool Mapped(Target target); /*<
	Returns true if the buffer currently bound to the specified
	[^target] is mapped.
	See [glfunc GetBufferParameter], [glconst BUFFER_MAPPED].
	>*/

	static void Resize(
		Target target,
		__BufferSize size,
		__BufferUsage usage = BufferUsage::StaticDraw
	); /*<
	Allocates or reallocates storage of a buffer currently bound
	to [^target] to the specified [^size] without uploading any data,
	taking [^usage] as a hint.
	See [glfunc BufferData].
	>*/

	static void RawData(
		Target target,
		__BufferSize size,
		const GLvoid* data,
		__BufferUsage usage = BufferUsage::StaticDraw
	); /*<
	Uploads [^size] bytes from the location pointed to by [^data]
	to the buffer bound to the specified [^target], taking [^usage]
	as a hint.
	See [glfunc BufferData].
	>*/

	static void Data(
		Target target,
		const __BufferData& data,
		__BufferUsage usage = BufferUsage::StaticDraw
	); /*<
	Uploads the specified [^data] to the buffer bound to the specified
	[^target] taking [^usage] as a hint.
	See [glfunc BufferData].
	>*/

	template <typename GLtype>
	static void Data(
		Target target,
		GLsizei count,
		const GLtype* data,
		__BufferUsage usage = BufferUsage::StaticDraw
	);

	static void SubData(
		Target target,
		__BufferSize offset,
		const __BufferData& data
	); /*< Uploads the specified [^data] to a sub-range (starting at
	[^offset]) of the buffer bound to the specified [^target].
	See [glfunc BufferSubData].
	>*/

	template <typename GLtype>
	static void SubData(
		Target target,
		__BufferSize offset,
		GLsizei count,
		const GLtype* data
	);

#if GL_VERSION_3_1 || GL_ARB_copy_buffer
	static void CopySubData(
		__BufferTarget readtarget,
		__BufferTarget writetarget,
		__BufferSize readoffset,
		__BufferSize writeoffset,
		__BufferSize size
	); /*<
	Copies a block of data of the specified [^size] from a buffer
	bound to [^readtarget] starting at [^readoffset] to a buffer
	bound to [^writetarget] starting at [^writeoffset].
	See [glfunc CopyBufferSubData].
	>*/
#endif

#if GL_VERSION_4_3
	template <typename GLtype>
	static void ClearData(
		Target target,
		__PixelDataInternalFormat internal_format,
		__PixelDataFormat format,
		const GLtype* value
	); /*<
	Clears the whole buffer bound to [^target] with the specified
	fixed [^value], in the specified [^format].
	See [glfunc ClearBufferSubData].
	>*/

	template <typename GLtype>
	static void ClearSubData(
		Target target,
		__PixelDataInternalFormat internal_format,
		__BufferSize offset,
		__BufferSize size,
		__PixelDataFormat format,
		const GLtype* value
	); /*< Clears a sub-range (specified by [^offset] and [^size])
	of a buffer bound to [^target] with the specified fixed [^value],
	using the specified [^internal_format].
	>*/
#endif

#if GL_VERSION_4_4 || GL_ARB_buffer_storage
	static void Storage(
		Target target,
		const __BufferData& data,
		__Bitfield<__BufferStorageBit> flags
	); /*<
	Creates a data store for buffer currently bound to
	[^target], and uploads the specified [^data].
	See [glfunc BufferStorage].
	>*/
	static void Storage(
		Target target,
		__BufferSize size,
		const void* data,
		__Bitfield<__BufferStorageBit> flags
	);

	static bool ImmutableStorage(Target target); /*<
	Returns true if the storage of the buffer currently bound
	to [^target] is immutable.
	See [glfunc GetBufferParameter], [glconst BUFFER_IMMUTABLE_STORAGE].
	>*/

	static __Bitfield<__BufferStorageBit> StorageFlags(Target target); /*<
	Returns the storage flags of a buffer currently bound to [^target].
	See [glfunc GetBufferParameter], [glconst BUFFER_STORAGE_FLAGS].
	>*/
#endif

// TODO
};

} // namespace oglplus
//]
