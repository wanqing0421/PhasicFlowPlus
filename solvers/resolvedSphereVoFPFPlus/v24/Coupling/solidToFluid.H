// Solid->Fluid interaction: Just the particles that are inside each CFD sub-domain are considered

const auto& pDiameters = coupling.particleDiameter();
const auto& pCentersOfMass = coupling.centerMass();
scalar resolution = readScalar(coupling.subDict("resolved").lookup("sphereSTLResolution"));

// Creating sphere STLs for each particle
PtrList<triSurface> particleSTLs(coupling.numParticles());

for(pFlow::uint32 i = 0; i < pDiameters.size(); i++)
{
	Foam::vector cm{pCentersOfMass[i].x(), 
		pCentersOfMass[i].y(), 
		pCentersOfMass[i].z()};

	particleSTLs.emplace_set(
		i, 
		generateSphere
		(
			cm,
			pDiameters[i],
			resolution
		)
	);
}

// Calculating the solid to fluid interaction
coupling.calculateSolidInteraction
(
	particleSTLs,
	particleIDPtr,
	Us
);

