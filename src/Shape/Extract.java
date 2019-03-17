//------------------------------------------------
public void Demo( int i ){
		switch( i ){
		case 1: Demo( Primitiv3d.SubNormalizeType.NORMALIZE ); break;
		case 2: Demo( Primitiv3d.SubNormalizeType.NORMALIZE_ONLY_SUB ); break;
		case 3: Demo( Primitiv3d.SubNormalizeType.NORMALIZE_HALF_INIT ); break;
		case 4: Demo( Primitiv3d.SubNormalizeType.NORMALIZE_DEC_INIT ); break;
		case 5: Demo( Primitiv3d.SubNormalizeType.NORMALIZE_INC_INIT ); break;
		case 6: Demo( Primitiv3d.SubNormalizeType.NORMALIZE_MUL_INIT ); break;
		case 7: Demo( Primitiv3d.SubNormalizeType.NORMALIZE_DEC_SUB ); break;
		case 8: Demo( Primitiv3d.SubNormalizeType.NORMALIZE_INC_SUB ); break;
		case 9: Demo( Primitiv3d.SubNormalizeType.NORMALIZE_MUL_SUB ); break;
		case 10: Demo( Primitiv3d.SubNormalizeType.NORMALIZE_ONLY_INIT ); break;
		case 11: Demo( Primitiv3d.SubNormalizeType.NORMALIZE_NONE ); break;
		}
}
//------------------------------------------------
public void	Demo( Primitiv3d.SubNormalizeType  pNormalize ) {
		// formes bien reguliere
		createDemoSprites( false, 0.3f, 0, pNormalize );
		createDemoSprites( false, 0.3f, 1, pNormalize );
		createDemoSprites( false, 0.5f, 2, pNormalize );
		createDemoSprites( false, 0.5f, 3, pNormalize );

		// formes regulieres mais originales avec decoupes
				
		createDemoSprites( true, 0.4f, 0, pNormalize  ); 
		createDemoSprites( true, 0.4f, 1, pNormalize ); 
		createDemoSprites( true, 0.4f, 2, pNormalize ); 
		createDemoSprites( true, 0.4f, 3, pNormalize ); 

				
		// formes cristalines ( doivent etre transparentes ! )
		createDemoSprites( false, -0.3f, 0, pNormalize );
		createDemoSprites( false, -0.3f, 1, pNormalize );
		createDemoSprites( false, -0.3f, 2, pNormalize );
		createDemoSprites( false, -0.3f, 3, pNormalize );

		// formes cristalines aussi mais plus exotiques
		createDemoSprites( true, -0.3f, 0, pNormalize );				
		createDemoSprites( true, -0.3f, 1, pNormalize );
		createDemoSprites( true, -0.3f, 2, pNormalize );	
		createDemoSprites( true, -0.3f, 3, pNormalize );	
}
//------------------------------------------------
public void createDemoSprites( boolean lCentralPoint, float lSize, int lDepth, Primitiv3d.SubNormalizeType  pNormalize ){

		lSize *= sGenSize;
				
		if( pNormalize != Primitiv3d.SubNormalizeType.NORMALIZE_INC_INIT )
				lSize *=2;
		else
				lSize *= 0.7f;

		Primitiv3d lPrim = 	new Primitiv3d();
		Primitiv3d.SubParamObject3d lParam01 = lPrim.GetSubParamObject3d ( 0+lDepth, lSize,       lCentralPoint, pNormalize );
		Primitiv3d.SubParamObject3d lParam02 = lPrim.GetSubParamObject3d ( 0+lDepth, lSize*1.01f, lCentralPoint, pNormalize );

		lParam01.cHoleFacet = 4;
		lParam01.cHoleDepth = 2;

		lParam01.cDepthGrowFactor = 0.5f;
		lParam02.cDepthGrowFactor = 0.5f;

		String cComment =  " Cp:" + lCentralPoint + " Sz:" +lSize +" Depth:"+ lDepth 	+ " Norm:" + pNormalize ; 

		Object3d  lCube1 =  Primitiv3d.Parallelepiped( lSize, lSize, lSize, lParam01).getObject3d();
		Object3d  lCube2 =  Primitiv3d.Parallelepiped( lSize*1.01f, lSize*1.01f, lSize*1.01f, lParam01).getObject3d();

		Object3d  lPyr1 =  Primitiv3d.Pyramid4( 0,0,0, lSize, lSize,  lParam01).getObject3d();
		Object3d  lPyr2 =  Primitiv3d.Pyramid4( 0,0,0, lSize*1.01f,  lSize*1.01f, lParam01).getObject3d();

				
		Primitiv3d.SubParamObject3d lParam11 = lPrim.GetSubParamObject3d ( 1+lDepth, lSize,        lCentralPoint, pNormalize );
		Primitiv3d.SubParamObject3d lParam12 = lPrim.GetSubParamObject3d ( 1+lDepth, lSize*1.003f, lCentralPoint, pNormalize );

		Primitiv3d.SubParamObject3d lParam21 = lPrim.GetSubParamObject3d ( 2+lDepth, lSize,        lCentralPoint, pNormalize );
		Primitiv3d.SubParamObject3d lParam22 = lPrim.GetSubParamObject3d ( 2+lDepth, lSize*1.003f, lCentralPoint, pNormalize );
				
		Object3d  lPrimTetrahedron01 =  GlutPrimitiv.Tetrahedron( lParam01).getObject3d();
		Object3d  lPrimTetrahedron02 =  GlutPrimitiv.Tetrahedron( lParam02).getObject3d();


		Object3d  lPrimOctahedron01 =  GlutPrimitiv.Octahedron( lParam01).getObject3d();
		Object3d  lPrimOctahedron02 =  GlutPrimitiv.Octahedron( lParam02).getObject3d();



		Object3d lPrimIcosahedron01 = GlutPrimitiv.Icosahedron( lParam01).getObject3d();
		Object3d lPrimIcosahedron02 = GlutPrimitiv.Icosahedron( lParam02 ).getObject3d();

				
		Object3d lPrimDodecahedron01 = GlutPrimitiv.Dodecahedron( lParam01).getObject3d();
		Object3d lPrimDodecahedron02 = GlutPrimitiv.Dodecahedron( lParam02 ).getObject3d();

						
		if( lSize < 0 || ( pNormalize == Primitiv3d.SubNormalizeType.NORMALIZE_NONE  && lDepth > 2) ){

				createSprite( lPrimTetrahedron01,  null, "Tetrahedron" + cComment);
				createSprite( lPrimOctahedron01,  null, "Octahedron" + cComment);
								
				createSprite( lPrimIcosahedron01,  null, "Icosahedron" + cComment);
				createSprite( lPrimDodecahedron01,  null, "Dodecahedron" + cComment );
		
				createSprite( lCube1, lCube2,	"Cube" + cComment );
				createSprite( lPyr1, lPyr2,	"Pyramide" + cComment ); 
		}else{
				createSprite( lPrimTetrahedron01,  lPrimTetrahedron02, "Tetrahedron" + cComment);
								
				createSprite( lPrimOctahedron01,  lPrimOctahedron02, "Octahedron" + cComment);
								
								
				createSprite( lPrimIcosahedron01, lPrimIcosahedron02, "Icosahedron" + cComment );
				createSprite( lPrimDodecahedron01, lPrimDodecahedron02, "Dodecahedron" + cComment   );

				createSprite( lCube1, lCube2,	"Cube" + cComment );
				createSprite( lPyr1, lPyr2,	"Pyramide" + cComment );
				

		}
}
//------------------------------------------------
//------------------------------------------------
//------------------------------------------------
