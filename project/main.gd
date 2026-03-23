extends Node3D

@onready var projectile: BPProjectile = $BPProjectile
@onready var camera: Camera3D = $Camera3D

var trail_mesh: ImmediateMesh
var trail_instance: MeshInstance3D
var trajectory: PackedVector3Array

func _ready():
	# grid
	_create_grid()

	# trail rendering
	trail_mesh = ImmediateMesh.new()
	trail_instance = MeshInstance3D.new()
	trail_instance.mesh = trail_mesh
	add_child(trail_instance)

	var mat = StandardMaterial3D.new()
	mat.albedo_color = Color.RED
	mat.shading_mode = BaseMaterial3D.SHADING_MODE_UNSHADED
	trail_instance.material_override = mat

	# signals
	projectile.projectile_landed.connect(_on_projectile_landed)
	# fire with angles: 45 deg elevation, 0 deg azimuth
	# muzzle_velocity set on the node
	trajectory.push_back(projectile.position)
	projectile.fire(45.0, 0.0)

func _process(_delta):
	var pos = projectile.position
	camera.position = Vector3(pos.x + 3.0, pos.y + 2.0, pos.z - 5.0)
	camera.look_at(pos)

	if projectile.is_active():
		trajectory.push_back(pos)
		_draw_trail()

func _on_projectile_landed():
	print("Landed at: ", projectile.position)

func _draw_trail():
	trail_mesh.clear_surfaces()
	if trajectory.size() < 2:
		return

	trail_mesh.surface_begin(Mesh.PRIMITIVE_LINE_STRIP)
	for point in trajectory:
		trail_mesh.surface_add_vertex(point)
	trail_mesh.surface_end()

func _create_grid():
	var mesh = ImmediateMesh.new()
	var inst = MeshInstance3D.new()
	inst.mesh = mesh

	var mat = StandardMaterial3D.new()
	mat.albedo_color = Color(0.5, 0.5, 0.5, 0.4)
	mat.shading_mode = BaseMaterial3D.SHADING_MODE_UNSHADED
	mat.transparency = BaseMaterial3D.TRANSPARENCY_ALPHA
	inst.material_override = mat

	var size = 2000
	mesh.surface_begin(Mesh.PRIMITIVE_LINES)
	for i in range(-size, size + 1, 20):
		# lines along Z
		mesh.surface_add_vertex(Vector3(i, 0, -size))
		mesh.surface_add_vertex(Vector3(i, 0, size))
		# lines along X
		mesh.surface_add_vertex(Vector3(-size, 0, i))
		mesh.surface_add_vertex(Vector3(size, 0, i))
	mesh.surface_end()

	add_child(inst)
