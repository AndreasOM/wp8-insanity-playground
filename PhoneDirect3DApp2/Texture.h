#pragma once
class Texture
{
	public:
					Texture(void);
		virtual		~Texture(void);
		void		initialize( int width, int height );
		void		shutdown();

		int			getWidth() const{ return m_width; }
		int			getHeight() const{ return m_height; }
		const void*	getData() const{ return m_pData; }

		void		noise( int rw, int rh );
	protected:
		int			m_width;
		int			m_height;
		void*		m_pData;
	private:
};

