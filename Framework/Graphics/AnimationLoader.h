#ifndef ANIMATION_LOADER 
#define ANIMATION_LOADER

#include <Math/Vec3.h>
#include <Math/Quat.h>
#include <Math/Matrix.h>

#include <string>

class BinaryReader;
class Animation;
template <typename T> class AnimationCurve;

class AnimationLoader
{
public:
	static Animation* LoadFromFile(const std::string &path);

private:
	static Animation* LoadAnimation(BinaryReader &br);

	static AnimationCurve<sm::Vec3>* LoadVec3Anim(BinaryReader &br, sm::Vec3 &localVec3);
	static AnimationCurve<sm::Quat>* LoadQuatAnim(BinaryReader &br, sm::Quat &localQuat, bool &ownAnim, float &angleScale);
	static sm::Matrix LoadMatrix(BinaryReader &br);
};

#endif // ANIMATION_LOADER

