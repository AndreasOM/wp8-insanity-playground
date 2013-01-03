#pragma once

// :TODO: we intentionally _don't_ abstract away the renderer and use D3D11
// this will change in the future, but keeps us focused on D3D11 now, which is the topic of this whole thing

class Model
{
	public:
					Model(void);
		virtual		~Model(void);
		bool		loadMD3( const char *pFilename );
	private:

};

