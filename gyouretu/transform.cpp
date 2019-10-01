#include "transform.h"
#include "common.h"


namespace Ato
{
	static int vertexCount = 4;
	static float scale = 1;
	static D3DCOLOR color = D3DCOLOR_RGBA(255, 255, 255, 255);
	static int scaleFromCenter = false;

	void ApplyScale(D3DXVECTOR4* pos);
	void ApplyScaleCenter(D3DXVECTOR4* pos);


	void ScaleFromCenter(bool center)
	{
		scaleFromCenter = center;
	}

	void SetScale(float value)
	{
		scale = value;
	}

	void SetColor(D3DCOLOR col)
	{
		color = col;
	}


	void ApplyScale(D3DXVECTOR4 *pos)
	{
		int originX = pos[0].x;
		int originY = pos[0].y;

		// get offset from origin
		pos[0].x *= scale;
		pos[0].y *= scale;

		int offsetX = originX - pos[0].x;
		int offsetY = originY - pos[0].y;

		pos[0].x = offsetX + pos[0].x;
		pos[0].y = offsetY + pos[0].y;

		for (int i = 1; i < vertexCount; i++)
		{
			// apply scale
			pos[i].x *= scale;
			pos[i].y *= scale;

			// offset to origin
			pos[i].x = offsetX + pos[i].x;
			pos[i].y = offsetY + pos[i].y;
		}
	}

	void ApplyScaleCenter(D3DXVECTOR4* pos)
	{
		float centerX = ((pos[1].x - pos[0].x) * 0.5F) + pos[0].x;
		float centerY = ((pos[2].y - pos[0].y) * 0.5F) + pos[0].y;

 		int width_offset = ((pos[1].x - pos[0].x) * scale) * 0.5F;
		int height_offset = ((pos[2].y - pos[0].y) * scale) * 0.5F;

		int mulX = 1, mulY = 1;
		for (int i = 0; i < vertexCount; i++)
		{
			// move to center
			pos[i].x = centerX;
			pos[i].y = centerY;

			mulX *= -1;
			if (i % 2 == 0) mulY *= -1;

			// offset from origin
			pos[i].x = pos[i].x + width_offset * mulX;
			pos[i].y = pos[i].y + height_offset * mulY;

		}
	}

	void DrawSprite(TextureIndex tex, int posX, int posY, int disp_width, int disp_height, float startX, float startY, float endX, float endY)
	{
		// get direct3d device
		LPDIRECT3DDEVICE9 device = MyDirect3D_GetDevice();

		// init d3dx vector4
		D3DXVECTOR4 v_pos[] = { D3DXVECTOR4(posX, posY, 0.0F, 1.0F), 
							    D3DXVECTOR4(posX + disp_width, posY, 0.0F, 1.0F),
								D3DXVECTOR4(posX, posY + disp_height, 0.0F, 1.0F),
								D3DXVECTOR4(posX + disp_width, posY + disp_height, 0.0F, 1.0F) };

		// init uv
		D3DXVECTOR2 uv_pos[] = { D3DXVECTOR2(startX, startY),
								 D3DXVECTOR2(endX, startY),
								 D3DXVECTOR2(startX, endY),
								 D3DXVECTOR2(endX, endY) };

		// apply scale
		if (scaleFromCenter)
			ApplyScaleCenter(v_pos);
		else
			ApplyScale(v_pos);
		
		// init vertex array
		Vertex2D_UV v[] = { {v_pos[0], color, uv_pos[0]},
							{v_pos[1], color, uv_pos[1]},
							{v_pos[2], color, uv_pos[2]},
							{v_pos[3], color, uv_pos[3]} };
		
		device->SetFVF(FVF_VERTEX2D);
		device->SetTexture(0, Texture_GetTexture(tex));
		device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2D_UV));
	}

}
