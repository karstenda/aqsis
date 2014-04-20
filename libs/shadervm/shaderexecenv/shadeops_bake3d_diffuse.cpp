// Aqsis
// Copyright (C) 1997 - 2001, Paul C. Gregory
//
// Contact: pgregory@aqsis.org
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


#ifdef AQSIS_SYSTEM_WIN32
#include <io.h>
#endif

#include <aqsis/util/logging.h>


#include <cstring>
#include "shaderexecenv.h"

#include <OpenEXR/ImathVec.h>


namespace Aqsis
{

using Imath::V3f;


//------------------------------------------------------------------------------
/// Currently just the same as calling the bake3d() shadeop.
///
void CqShaderExecEnv::SO_bake3d_diffuse( IqShaderData* ptc,
                                 IqShaderData* Position,
                                 IqShaderData* Normal,
                                 IqShaderData* surfCol,
                                 IqShaderData* area,
                                 IqShaderData* Result,
                                 IqShader* pShader,
                                 TqInt cParams,
                                 IqShaderData** apParams ) {


	/**
	 * Find categories param.
	 */
	// Holding the categories of the lights to use.
	IqShaderData* category = NULL;
	CqString paramName;
	for (int i = 0; i < cParams; i += 2) {
		if (apParams[i]->Type() == type_string) {
			apParams[i]->GetString(paramName);
			IqShaderData* paramValue = apParams[i + 1];
			if (paramName == "_category") {
				category = paramValue;
			}
		}
	}

	/**
	 * Calculate the diffuse color with a shadeop call to SO_diffuse.
	 */
	CqShaderExecEnv::SO_diffuse(Normal,category,Result,pShader);


	/**
	 * Apply the surface color.
	 */
	for (int igrid=0; igrid <shadingPointCount(); igrid++) {

		// apply surface color
		CqColor rad, Cs;
		Result->GetColor(rad,igrid);
		surfCol->GetColor(Cs, igrid);
		rad *= Cs;

		Result->SetColor(rad,igrid);
	}

	/*
	 * Create the various parameters to the SO_bake3d call.
	 */
	// the name parameter of radiosity.
	IqShaderData* nameRad = pShader->CreateTemporaryStorage(type_string,
				class_uniform);

	// The var  + param name indicating interpolation.
	IqShaderData* nameInterpolate = pShader->CreateTemporaryStorage(
			type_string, class_uniform);
	IqShaderData* interpolate = pShader->CreateTemporaryStorage(type_bool,
			class_uniform);

	// The name parameter of the area.
	IqShaderData* nameArea = pShader->CreateTemporaryStorage(type_string,
			class_uniform);

	// The var holding the end result of SO_bake3d.
	IqShaderData* ResultBake3d = pShader->CreateTemporaryStorage(type_float,
			class_varying);

	// Check if all parameters are assigned.
	if (NULL == nameRad || NULL == nameInterpolate || NULL == interpolate
			|| NULL == nameArea || NULL == ResultBake3d) {
		Aqsis::log() << error
				<< "bake_diffuse: Not able to reserve memory for parameters to bake3d call."
				<< std::endl;
		return;
	}


	/*
	 * Initiate these parameters.
	 */

	// Initiate the name var of radiosity
	nameRad->SetString("_radiosity");

	// Initiate the var indicating interpolate + name.
	nameInterpolate->SetString("interpolate");
	interpolate->SetBool(true);

	// Initiate the name var of area
	nameArea->SetString("_area");

	// Initiare the size of Result.
	ResultBake3d->SetSize(shadingPointCount());

	// Put the right parameters in the array of apParams.
	TqInt cParamsBake3d = 6;
	IqShaderData* apParamsBake3d[cParamsBake3d];
	apParamsBake3d[0] = nameRad;
	apParamsBake3d[1] = Result;
	apParamsBake3d[2] = nameArea;
	apParamsBake3d[3] = area;
	apParamsBake3d[4] = nameInterpolate;
	apParamsBake3d[5] = interpolate;

	/*
	 *  Make the call to bake3d.
	 */
	CqShaderExecEnv::SO_bake3d(ptc,NULL,Position,Normal,ResultBake3d,pShader,cParamsBake3d,apParamsBake3d);

	/*
	 * Delete all the temporary parameters afterwards.
	 */
	pShader->DeleteTemporaryStorage(ResultBake3d);
	pShader->DeleteTemporaryStorage(nameArea);
	pShader->DeleteTemporaryStorage(interpolate);
	pShader->DeleteTemporaryStorage(nameInterpolate);
	pShader->DeleteTemporaryStorage(nameRad);


}
//---------------------------------------------------------------------

} // namespace Aqsis

