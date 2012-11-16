

#include	<string>
#include	<stdio.h>

#include	<aqsis/math/math.h>
#include	"shaderexecenv.h"
#include	<aqsis/core/ilightsource.h>

#include	"../../pointrender/microbuffer.h"


namespace Aqsis {

void CqShaderExecEnv::SO_indirect(IqShaderData* P, IqShaderData* N,
										 IqShaderData* samples,
										 IqShaderData* result,
										 IqShader* pShader, int cParams,
										 IqShaderData** apParams)
{
	result->SetColor(CqColor(0,0,0),0);
}

}
